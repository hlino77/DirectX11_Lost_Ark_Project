#include "InstanceData.h"

InstanceDataDesc::~InstanceDataDesc()
{
	Safe_Delete(pInstanceValue);
	Safe_Release(pInstanceBuffer);
	Safe_Release(pInstanceShader);

	Safe_Delete(pAnimInstanceValue);
	Safe_Release(pAnimInstanceTexture);
	Safe_Release(pAnimSRV);
}
