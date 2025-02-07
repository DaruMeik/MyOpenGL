#include "../Game/game_object.h"

class Player : public GameObject
{
public:
	using GameObject::GameObject;

	void Start() override;
	void Update(std::vector<bool>& input) override;
private:
	bool m_ReadyToJump = true;
};