#pragma once

#include "Common.h"
#include "ShadingMethodBase.h"

AWAY_NAMESPACE_BEGIN

/**
 * LightingMethodBase provides an abstract base method for shading methods that uses lights.
 * Used for diffuse and specular methods only.
 */
class LightingMethodBase : public ShadingMethodBase
{
public:
	LightingMethodBase();

	/**
	 * Get the fragment shader code that will be needed before any per-light code is added.
	 *
	 * @param vo The MethodVO object containing the method data for the currently compiled material pass.
	 * @param regCache The register cache used during the compilation.
	 */
	virtual void getFragmentPreLightingCode(ShaderChunk& code, MethodVO* vo, ShaderRegisterCache* regCache);

	/**
	 * Get the fragment shader code that will generate the code relevant to a single light.
	 *
	 * @param vo The MethodVO object containing the method data for the currently compiled material pass.
	 * @param lightDirReg The register containing the light direction vector.
	 * @param lightColReg The register containing the light color.
	 * @param regCache The register cache used during the compilation.
	 */
	virtual void getFragmentCodePerLight(ShaderChunk& code, MethodVO* vo, unsigned int lightDirReg, unsigned int lightColReg, ShaderRegisterCache* regCache);

	/**
	 * Get the fragment shader code that will generate the code relevant to a single light probe object.
	 *
	 * @param vo The MethodVO object containing the method data for the currently compiled material pass.
	 * @param cubeMapReg The register containing the cube map for the current probe
	 * @param weightRegister A string representation of the register + component containing the current weight
	 * @param regCache The register cache providing any necessary registers to the shader
	 */
	virtual void getFragmentCodePerProbe(ShaderChunk& code, MethodVO* vo, unsigned int cubeMapReg, unsigned int weightRegister, ShaderRegisterCache* regCache);

	/**
	 * Get the fragment shader code that should be added after all per-light code. Usually composits everything to the target register.
	 *
	 * @param vo The MethodVO object containing the method data for the currently compiled material pass.
	 * @param regCache The register cache used during the compilation.
	 * @param targetReg The register containing the final shading output.
	 */
	virtual void getFragmentPostLightingCode(ShaderChunk& code, MethodVO* vo, ShaderRegisterCache* regCache, unsigned int targetReg);

	/**
	 * A method that is exposed to wrappers in case the strength needs to be controlled
	 */
	ModulateFunc m_modulateMethod;
};

AWAY_NAMESPACE_END