#include "Component.h"
#include "Actor.h"

Component::Component(Actor* owner, int updateOrder)
	:mOwner(owner),
	mUpdateOrder(updateOrder) {
	// �A�N�^�[�̃R���|�[�l���g��vector��ǉ�
	mOwner->AddComponent(this);
}

Component::~Component() {
	mOwner->RemoveComponent(this);
}

void Component::Update(float deltaTime) {

}