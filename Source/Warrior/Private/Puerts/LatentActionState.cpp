// sumikiru All Rights Reserved


#include "Puerts/LatentActionState.h"

void ULatentActionState::OnLatentActionCompleted(int32 LinkID)
{
	LatentActionCallback.ExecuteIfBound();
}

FLatentActionInfo ULatentActionState::GetLatentActionInfo()
{
	return FLatentActionInfo(
		0, /* The resume point within the function to execute */
		GetTypeHash(FGuid::NewGuid()), /* the UUID for this action */
		TEXT("OnLatentActionCompleted"), /* 要执行的函数，在Latent节点（如Delay）设置的Duration之后执行 */
		this /* Object to execute the function on. */
	);
}
