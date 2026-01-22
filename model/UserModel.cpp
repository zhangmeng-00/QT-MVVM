#include "UserModel.h"
#include "core/policy/AlwaysPolicy.h"

void UserModel::SetupSubscriptions()
{
  Subscribe("user/score", std::make_shared<AlwaysPolicy>());
}
