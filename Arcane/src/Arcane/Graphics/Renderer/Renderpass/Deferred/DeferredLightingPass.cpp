#include "arcpch.h"
#include "DeferredLightingPass.h"

#include <Arcane/Graphics/Window.h>
#include <Arcane/Graphics/Shader.h>
#include <Arcane/Graphics/Renderer/GLCache.h>
#include <Arcane/Graphics/Camera/ICamera.h>
#include <Arcane/Graphics/Renderer/Renderer.h>
#include <Arcane/Graphics/Renderer/Renderpass/Deferred/DeferredGeometryPass.h>
#include <Arcane/Scene/Scene.h>
#include <Arcane/Util/Loaders/ShaderLoader.h>

namespace Arcane
{
	DeferredLightingPass::DeferredLightingPass(Scene *scene) : RenderPass(scene), m_AllocatedFramebuffer(true)
	{
		m_LightingShader = ShaderLoader::LoadShader("deferred/PBR_LightingPass.glsl");

		m_Framebuffer = new Framebuffer(Window::GetRenderResolutionWidth(), Window::GetRenderResolutionHeight(), false);
		m_Framebuffer->AddColorTexture(FloatingPoint16).AddDepthStencilTexture(NormalizedDepthStencil).CreateFramebuffer();
	}

	DeferredLightingPass::DeferredLightingPass(Scene *scene, Framebuffer *customFramebuffer) : RenderPass(scene), m_AllocatedFramebuffer(false), m_Framebuffer(customFramebuffer)
	{
		m_LightingShader = ShaderLoader::LoadShader("deferred/PBR_LightingPass.glsl");
	}

	DeferredLightingPass::~DeferredLightingPass()
	{
		if (m_AllocatedFramebuffer) {
			delete m_Framebuffer;
		}
	}

	LightingPassOutput DeferredLightingPass::ExecuteLightingPass(ShadowmapPassOutput &inputShadowmapData, GBuffer *inputGbuffer, PreLightingPassOutput &preLightingOutput, ICamera *camera, bool useIBL)
	{
		// Framebuffer setup
		glViewport(0, 0, m_Framebuffer->GetWidth(), m_Framebuffer->GetHeight());
		glViewport(0, 0, m_Framebuffer->GetWidth(), m_Framebuffer->GetHeight());
		m_Framebuffer->Bind();
		m_Framebuffer->ClearAll();
		m_GLCache->SetDepthTest(false);
		m_GLCache->SetMultisample(false);

		// Move the depth + stencil of the GBuffer to our framebuffer
		// NOTE: Framebuffers have to have identical depth + stencil formats for this to work
		glBindFramebuffer(GL_READ_FRAMEBUFFER, inputGbuffer->GetFramebuffer());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Framebuffer->GetFramebuffer());
		glBlitFramebuffer(0, 0, inputGbuffer->GetWidth(), inputGbuffer->GetHeight(), 0, 0, m_Framebuffer->GetWidth(), m_Framebuffer->GetHeight(), GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);

		// Setup initial stencil state
		m_GLCache->SetStencilTest(true);
		m_GLCache->SetStencilWriteMask(0x00); // Do not update stencil values

		LightManager *lightManager = m_ActiveScene->GetLightManager();
		ProbeManager *probeManager = m_ActiveScene->GetProbeManager();

		m_GLCache->SetShader(m_LightingShader);
		lightManager->BindLightingUniforms(m_LightingShader);
		m_LightingShader->SetUniform("viewPos", camera->GetPosition());
		m_LightingShader->SetUniform("viewInverse", glm::inverse(camera->GetViewMatrix()));
		m_LightingShader->SetUniform("projectionInverse", glm::inverse(camera->GetProjectionMatrix()));

		// Bind GBuffer data
		inputGbuffer->GetAlbedo()->Bind(5);
		m_LightingShader->SetUniform("albedoTexture", 5);

		inputGbuffer->GetNormal()->Bind(6);
		m_LightingShader->SetUniform("normalTexture", 6);

		inputGbuffer->GetMaterialInfo()->Bind(7);
		m_LightingShader->SetUniform("materialInfoTexture", 7);

		preLightingOutput.ssaoTexture->Bind(8);
		m_LightingShader->SetUniform("ssaoTexture", 8);

		inputGbuffer->GetDepthStencilTexture()->Bind(9);
		m_LightingShader->SetUniform("depthTexture", 9);

		m_LightingShader->SetUniform("nearPlane", NEAR_PLANE);
		m_LightingShader->SetUniform("farPlane", FAR_PLANE);

		// Shadowmap code
		BindShadowmap(m_LightingShader, inputShadowmapData);

		// Finally perform the lighting using the GBuffer

		// IBL Bindings
		glm::vec3 cameraPosition = camera->GetPosition();
		probeManager->BindProbes(cameraPosition, m_LightingShader); // TODO: Should use camera component

		// Perform lighting on the terrain (turn IBL off)
		m_LightingShader->SetUniform("computeIBL", 0);
		m_GLCache->SetStencilFunc(GL_EQUAL, StencilValue::TerrainStencilValue, 0xFF);
		Renderer::DrawNdcPlane();

		// Perform lighting on the models in the scene
		if (useIBL)
		{
			m_LightingShader->SetUniform("computeIBL", 1);
		}
		else
		{
			m_LightingShader->SetUniform("computeIBL", 0);
		}
		m_GLCache->SetStencilFunc(GL_EQUAL, StencilValue::ModelStencilValue, 0xFF);
		Renderer::DrawNdcPlane();


		// Reset state
		m_GLCache->SetDepthTest(true);
		m_GLCache->SetStencilTest(false);

		// Render pass output
		LightingPassOutput passOutput;
		passOutput.outputFramebuffer = m_Framebuffer;
		return passOutput;
	}

	void DeferredLightingPass::BindShadowmap(Shader *shader, ShadowmapPassOutput &shadowmapData)
	{
		bool hasDirShadowMap = shadowmapData.directionalShadowmapFramebuffer != nullptr;
		bool hasSpotShadowMap = shadowmapData.spotLightShadowmapFramebuffer != nullptr;

		shader->SetUniform("dirLightShadowData.hasShadow", hasDirShadowMap);
		shader->SetUniform("spotLightShadowData.hasShadow", hasSpotShadowMap);

		if (hasDirShadowMap)
		{
			shadowmapData.directionalShadowmapFramebuffer->GetDepthStencilTexture()->Bind(0);
			shader->SetUniform("dirLightShadowmap", 0);
			shader->SetUniform("dirLightShadowData.lightSpaceViewProjectionMatrix", shadowmapData.directionalLightViewProjMatrix);
			shader->SetUniform("dirLightShadowData.shadowBias", shadowmapData.directionalShadowmapBias);
		}
		if (hasSpotShadowMap)
		{
			shadowmapData.spotLightShadowmapFramebuffer->GetDepthStencilTexture()->Bind(1);
			shader->SetUniform("spotLightShadowmap", 1);
			shader->SetUniform("spotLightShadowData.lightSpaceViewProjectionMatrix", shadowmapData.spotLightViewProjMatrix);
			shader->SetUniform("spotLightShadowData.shadowBias", shadowmapData.spotLightShadowmapBias);
		}
	}
}
