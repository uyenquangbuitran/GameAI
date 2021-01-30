#include "NPC.h"

#include "SpriteList.h"
#include "GameCollision.h"
#include "GameDefine.h"
#include "Astar.h"
#include "NPCDecisionMaker.h"

NPC::NPC()
{
	IsDeleted = false;
	_width = 25;
	_height = 25;

	//khởi tạo các animation
	_leftAnimation = new Animation();
	_rightAnimation = new Animation();
	_upAnimation = new Animation();
	_downAnimation = new Animation();

	_shieldAnimation = new Animation(0.07f);
	_shieldAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 0, 0 + 39, 111, 111 + 40, D3DXVECTOR2(20.f, 20.f)));
	_shieldAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 39, 39 + 40, 111, 111 + 40, D3DXVECTOR2(20.f, 20.f)));

	// mặc định animation ban đầu
	_currentAnimation = _leftAnimation;
	Position = D3DXVECTOR2(300.f, 300.f);
	_isMoving = true;
	_isShield = false;

	// tạo 6 bullet cho npc
	for (int i = 0; i < 4; i++)
	{
		Bullet* bullet = new Bullet();
		_bulletList.push_back(bullet);
	}

	_isAutoShoot = true;
}

void NPC::SetPosition(float x, float y)
{
	Position = D3DXVECTOR2(x, y);

	AStar::getInstance()->SetValue(x / X_STEP, y / Y_STEP, ASTAR_VALUE_NPC);
}

void NPC::Update(float _dt)
{
	if (IsDeleted) return;

	_attackRangeFactor = NPCDecisionMaker::GetInstance()->GetAttackRangeFactor();
	_attackRatioFactor = NPCDecisionMaker::GetInstance()->GetAttackRatioFactor();

	if (!_isCollision)
		Position += Velocity * _dt;

	AutoMove(_dt);
	AutoShoot(_dt);

	if (_isShield)
	{
		_shieldAnimation->Update(_dt);

		_shieldedTime += _dt;
		if (_shieldedTime > SHIELD_TIME_MAX) {
			_isShield = false;
			_shieldedTime = 0;
		}
	}
}

void NPC::DrawPath()
{
	if (IS_DRAW_ASTAR_PATH && !path.empty())
	{
		for (auto grid : path)
		{
			grid->Draw();
		}

		//draw des
		AStar::getInstance()->mapGrid[_lastDesPos->x][_lastDesPos->y]->Draw();
	}
}

void NPC::Draw()
{
	if (IsDeleted)
		return;

	_currentAnimation->Draw(Position);

	if (_isShield)
		_shieldAnimation->Draw(Position);
}

bool NPC::CheckCollision(Entity * e)
{
	if (e->IsDeleted) return false;

	//check collision bullet
	for (auto bullet : _bulletList)
	{
		bullet->CheckCollision(e);
	}

	if (IsDeleted)
		return false;

	//kiểm tra vị trí không thể va chạm khỏi xét
	if (!CanCollision(e)) return false;


	CollisionResult cR = GameCollision::getCollisionResult(this, e);
	if (cR.IsCollided)
	{
		//dừng lại
		Stop();

		//tách ra khỏi va chạm
		if (cR.Side == CS_Left)
		{
			Position.x += (float)(cR.Rect.right - cR.Rect.left) + 1;
		}
		else if (cR.Side == CS_Right)
		{
			Position.x -= (float)(cR.Rect.right - cR.Rect.left) - 1;
		}
		else if (cR.Side == CS_Top)
		{
			Position.y += (float)(cR.Rect.bottom - cR.Rect.top) + 1;
		}
		else if (cR.Side == CS_Bottom)
		{
			Position.y -= (float)(cR.Rect.bottom - cR.Rect.top) - 1;
		}

		//kiểm tra node hiện tại, node trước => valid sẽ tốc biến về đó
		if (!path.empty())
		{
			for (int i = -1; i <= 0; i++)
			{
				if (currentNodeIndex + i < path.size())
				{
					if (AStar::getInstance()->isValidAndNotObs(path.at(currentNodeIndex + i)->GetVec().x,
						path.at(currentNodeIndex + i)->GetVec().y))
					{
						Position = path.at(currentNodeIndex + i)->GetPosition();
						break;
					}
				}
			}
		}
		if (!_isCollision && !_isDodging)
		{
			RunDodging(); //chạy né va chạm		
		}

		if (e->getType() == ET_ProtectItem)
		{
			//ăn item bảo vệ
			_isShield = true;
			e->IsDeleted = true;
		}
		else if (e->getType() == ET_HealItem)
		{
			//ăn item máu
			_hp += e->_hp;
			if (_hp > _maxHP) _hp = _maxHP;
			e->IsDeleted = true;
		}
	}
	return cR.IsCollided;
}

void NPC::ApplyAnimation()
{
	switch (_direction)
	{
	case D_Left:
		_currentAnimation = _leftAnimation;
		break;
	case D_Right:
		_currentAnimation = _rightAnimation;
		break;
	case D_Up:
		_currentAnimation = _upAnimation;
		break;
	case D_Down:
		_currentAnimation = _downAnimation;
		break;
	default:
		break;
	}
}

D3DXVECTOR2 NPC::GetVelocityByDirection(Direction direction)
{
	if (direction == D_Stand)
	{
		return D3DXVECTOR2(0, 0);
	}
	else if (direction == D_Left)
	{
		return D3DXVECTOR2(-_speed, 0);
	}
	else if (direction == D_Right)
	{
		return D3DXVECTOR2(_speed, 0);
	}
	else if (direction == D_Up)
	{
		return D3DXVECTOR2(0, -_speed);
	}
	else if (direction == D_Down)
	{
		return D3DXVECTOR2(0, _speed);
	}

	return D3DXVECTOR2(0, 0);
}

void NPC::ApplyVelocity()
{
	switch (_direction)
	{
	case D_Stand:
		Velocity = D3DXVECTOR2(0.f, 0.f);
		break;
	case D_Left:
		Velocity = D3DXVECTOR2(-_speed, 0.f);
		break;
	case D_Right:
		Velocity = D3DXVECTOR2(_speed, 0.f);
		break;
	case D_Up:
		Velocity = D3DXVECTOR2(0.f, -_speed);
		break;
	case D_Down:
		Velocity = D3DXVECTOR2(0.f, _speed);
		break;
	}
}

template < typename T>
std::pair<bool, int > findInVector(const std::vector<T>  & vecOfElements, const T  & element)
{
	std::pair<bool, int > result;
	// Find given element in vector
	auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);
	if (it != vecOfElements.end())
	{
		result.second = distance(vecOfElements.begin(), it);
		result.first = true;
	}
	else
	{
		result.first = false;
		result.second = -1;
	}
	return result;
}


void NPC::AutoMove(float dt)
{
	if (_isCollision)
	{
		Stop();
		_timeWaittingMove += dt;
		if (_timeWaittingMove > 2) //đợi 2s rời va chạm
		{
			_timeWaittingMove = 0;
			RunDodging();
			return;
		}
		return;
	}

	if (_state == NPCS_Attacking)
	{
		_timeWaittingMove += dt;
		if (_timeWaittingMove > 2) //đợi 2s bắn
		{
			_timeWaittingMove = 0;
			_state = NPCS_RunAstar;
			return;
		}
		return;
	}

	if (_isEnemyInRange && _state != NPCS_RunAttack)
	{
		int posX, posY;
		posX = (Position.x / X_STEP);
		posY = (Position.y / Y_STEP);

		if (_state == NPCS_RunCollect)
		{
			_state = NPCS_RunAttack;
			//đi tới item
			PathAStar(vecPlayer->x, vecPlayer->y);
		}
		else {
			_state = NPCS_RunAttack;
			//sort từ gần đến xa NPC (ưu tiên đi tới điểm gần)
			int xRound = posX < vecPlayer->x ? posX - _attackRange * _attackRangeFactor
											: posX + _attackRange * _attackRangeFactor;
			int yRound = posY < vecPlayer->y ? posY - _attackRange * _attackRangeFactor
											: posY + _attackRange * _attackRangeFactor;
			std::vector<Vector2D> validList = AStar::getInstance()->GetListVecInAxisValid(vecPlayer->x, vecPlayer->y,
				xRound, yRound, 0);
			Vector2D vecAttack;
			if (!validList.empty())
			{
				//sort lại theo list có khoảng cách nhỏ nhất (buble sort)
				for (int i = 0; i < validList.size(); i++)
				{
					// Last i elements are already in place  
					for (int j = 0; j < validList.size() - i - 1; j++)
					{
						if (vec->distance(validList.at(i)) < vec->distance(validList.at(j)))
						{
							//thay đổi giá trị
							Vector2D vecTemp;
							vecTemp.x = validList.at(i).x;
							vecTemp.y = validList.at(i).y;

							validList.at(i).x = validList.at(j).x;
							validList.at(i).y = validList.at(j).y;

							validList.at(j).x = vecTemp.x;
							validList.at(j).y = vecTemp.y;
						}
					}
				}

				for (auto vec : validList)
				{
					PathAStar(vec.x, vec.y);
					if (!path.empty())
					{
						printLog("Move attack");
						vecAttack.x = vec.x;
						vecAttack.y = vec.y;
						break;
					}
				}

				//xoay mặt về hướng player, lưu hướng bắn
				if (vecPlayer->x == vecAttack.x)
				{
					_attackDirection = vecPlayer->y > vecAttack.y ? D_Down : D_Up;
				}
				else if (vecPlayer->y == vecAttack.y)
				{
					_attackDirection = vecPlayer->x > vecAttack.x ? D_Right : D_Left;
				}
			}

		}

	}

	if (!path.empty())
	{
		if (currentNodeIndex >= path.size())
		{
			if (_state != NPCS_RunAttack)
			{
				RunAstar();
				//_ = false;
			}
			else
			{
				_state = NPCS_Attacking; //đứng lại bắn player
				_direction = _attackDirection;
				_shotDirection = _attackDirection;
				ApplyAnimation();
				Stop();
			}
		}
		else
		{
			Move(path[currentNodeIndex]->GetPosition());
			if (!_isMoving)
			{
				//bỏ giá trị node đuôi
				AStar::getInstance()->SetValue(path[currentNodeIndex]->GetVec().x, path[currentNodeIndex]->GetVec().y, 0);

				//tăng node
				currentNodeIndex++;

				//set giá trị node mới
				if (currentNodeIndex < path.size())
					AStar::getInstance()->SetValue(path[currentNodeIndex]->GetVec().x,
													path[currentNodeIndex]->GetVec().y, ASTAR_VALUE_NPC);
			}
		}
	}
	else
	{
		if (_state != NPCS_RunAttack)
		{
			RunAstar();
		}
		else
		{
			_state = NPCS_Attacking; //đứng lại bắn player
				//xoay mặt về hướng player
			_direction = _attackDirection;
			_shotDirection = _attackDirection;
			ApplyAnimation();
			Stop();
		}
	}
}


void NPC::PredictCollision()
{
	if (_isMoving && !_isCollision && !_isDodging)
	{
		if (!path.empty())
		{
			//dự đoán những nốt tiếp theo có vật cản không, nếu có random lại vị trí di chuyển 1 path nhỏ
			//giữ des cũ, khi chạy xong des mới => chạy lại des cũ
			if (currentNodeIndex < path.size())
			{
				for (unsigned i = currentNodeIndex + 1; i < currentNodeIndex + 4; i++)
				{
					if (i >= path.size()) break;
					if (AStar::getInstance()->isObstacle(path.at(i)->GetVec().x, path.at(i)->GetVec().y,
						AStar::getInstance()->map))
					{
						RunDodging();
						break;
					}
				}
			}
		}
	}
}

void NPC::RunDodging()
{
	Stop();

	Vector2D *pos = new Vector2D();
	Vector2D pos0;
	pos0.x = Position.x / X_STEP;
	pos0.y = Position.y / Y_STEP;

	//nếu còn va chạm kiểm tra 9 node xung quanh, node nào có thể tới => tốc biến tới node đó (node không có va chạm)
	if (_isCollision)
	{
		Vector2D *pos = new Vector2D();
		if (AStar::getInstance()->RandomValidPosAround(pos0, pos, 1))
		{
			//tốc biến tách va chạm
			SetPosition((float)(pos->x + 0.5f)* X_STEP, (float)(pos->y + 0.5f)* Y_STEP);
		}
	}

	//random 1 node xung quanh, bán kính 3 ô
	if (AStar::getInstance()->RandomValidPosAround(pos0, pos, 3)) {
		//random path xung quanh

		_isDodging = true;
		_state = NPCS_RunDodging;
		PathAStar(pos->x, pos->y);
	}
}

void NPC::RunRandom()
{
	Vector2D *pos = new Vector2D();
	if (AStar::getInstance()->RamdomValidPosition(pos))
	{
		PathAStar(pos->x, pos->y);
	}
}

void NPC::RunAstar()
{
	_state = NPCS_RunAstar;
	if (!_isDodging) //đang không phải né
	{
		//tiếp tục đi con đường dang dở :D
		if (!path.empty())
		{
			if (currentNodeIndex >= path.size())
			{
				if (path.at(path.size() - 1)->GetVec().x == _lastDesPos->x
					&& path.at(path.size() - 1)->GetVec().y == _lastDesPos->y)
				{
					RunRandom();
				}
				else
				{
					PathAStar(_lastDesPos->x, _lastDesPos->y);
				}
			}
			else
			{
				if (path.at(currentNodeIndex)->GetVec().x == _lastDesPos->x
					&& path.at(currentNodeIndex)->GetVec().y == _lastDesPos->y)
				{
					RunRandom();
				}
				else
				{
					PathAStar(_lastDesPos->x, _lastDesPos->y);
				}
			}
		}
		else //random điểm mới
		{
			RunRandom();
		}

	}
	else //đã né xong, đi về chỗ đã lưu
	{
		_isDodging = false;
		//hết né, tính astar về chỗ ban đầu
		PathAStar(_lastDesPos->x, _lastDesPos->y);
	}
}

void NPC::PathAStar(int posX, int posY)
{
	currentNodeIndex = 0;

	Node beginNode;
	beginNode.SetX(Position.x / X_STEP);
	beginNode.SetY(Position.y / Y_STEP);

	Node destNode;
	destNode.SetX(posX);
	destNode.SetY(posY);

	if (_state == NPCS_RunAstar)
	{
		if (IS_DRAW_ASTAR_PATH)
			AStar::getInstance()->mapGrid[_lastDesPos->x][_lastDesPos->y]->SetType(Empty);

		//lưu lại điểm cuối
		_lastDesPos->x = posX;
		_lastDesPos->y = posY;

		if (IS_DRAW_ASTAR_PATH)
			AStar::getInstance()->mapGrid[_lastDesPos->x][_lastDesPos->y]->SetType(Destination);
	}

	AstarResult resuit = AStar::getInstance()->findPathV2(beginNode, destNode);

	//draw closed, opened list

	/*for (auto node :resuit.closeList)
	{
		Astar::getInstance()->mapGrid[node.x][node.y]->SetType(Closed);
	}

	for (auto node : resuit.openList)
	{
		Astar::getInstance()->mapGrid[node.x][node.y]->SetType(Opened);
	}*/

	//xóa đường vẽ path cũ
	if (IS_DRAW_ASTAR_PATH && !path.empty())
	{
		for (auto grid : path)
		{
			grid->SetType(Empty);
		}
	}

	//set path
	path.clear();

	for (auto node : resuit.path)
	{
		if (IS_DRAW_ASTAR_PATH)
		{
			switch (_state)
			{
			case NPCS_RunAstar:
				if (AStar::getInstance()->mapGrid[node.x][node.y]->GetType() != Destination)
					AStar::getInstance()->mapGrid[node.x][node.y]->SetType(Path);
				break;
			case NPCS_RunDodging:
				if (AStar::getInstance()->mapGrid[node.x][node.y]->GetType() != Destination)
					AStar::getInstance()->mapGrid[node.x][node.y]->SetType(Path_Dodging);
				break;
			case NPCS_RunAttack:
				if (AStar::getInstance()->mapGrid[node.x][node.y]->GetType() != Destination)
					AStar::getInstance()->mapGrid[node.x][node.y]->SetType(Path_Attacking);
				break;
			}
		}

		path.push_back(AStar::getInstance()->mapGrid[node.x][node.y]);
	}

	_isMoving = true;
}

void NPC::Move(D3DXVECTOR2 destination)
{
	if (abs(destination.x - Position.x) < 2)
	{
		destination.x = round(destination.x);
		Position.x = destination.x;
	}

	if (abs(destination.y - Position.y) < 2)
	{
		destination.y = round(destination.y);
		Position.y = destination.y;
	}

	if (destination.x - Position.x < -0.5)
	{
		_direction = D_Left;
		_shotDirection = D_Left;
		_isMoving = true;
	}
	else if (destination.x - Position.x > 0.5)
	{
		_direction = D_Right;
		_shotDirection = D_Right;
		_isMoving = true;
	}
	else if (destination.y - Position.y < -0.5)
	{
		_direction = D_Up;
		_shotDirection = D_Up;
		_isMoving = true;
	}
	else if (destination.y - Position.y > 0.5)
	{
		_direction = D_Down;
		_shotDirection = D_Down;
		_isMoving = true;
	}
	else
	{
		_direction = D_Stand;
		_isMoving = false;
	}

	_timeWaittingMove = 0;

	ApplyAnimation();
	ApplyVelocity();
	PredictCollision();

	//cập nhật vec
	vec->x = Position.x / X_STEP;
	vec->y = Position.y / Y_STEP;
}

void NPC::Stop()
{
	_direction = D_Stand;
	_isMoving = false;
	ApplyAnimation();
	ApplyVelocity();
	printLog("Stop");
}

void NPC::SetIsCollision(bool isCollision) {
	if (!_isCollision && isCollision)
	{
		Stop();
		RunDodging();
	}
	_isCollision = isCollision;
}

void NPC::MoveGridAstar(int x, int y)
{
	if (_isDivingToPlayer) return;
		
	_lastDesPos->x = x;
	_lastDesPos->y = y;
	
	_state = NPCS_RunAstar;
	PathAStar(_lastDesPos->x, _lastDesPos->y);
}

void NPC::AutoShoot(float dt)
{
	if (!_isAutoShoot) return;

	_timeWaittingShoot += dt;
	float _timeDelay = (_state == NPCS_RunAttack || _state == NPCS_Attacking) ? _shotCooldown / 2 : _shotCooldown;
	if (_timeWaittingShoot > _timeDelay)
	{
		_timeWaittingShoot = 0;
		//shot
		//shoot bullet
		if (!_bulletList.empty() && _shotDirection != D_Stand)
		{
			if (_currentBullet >= _bulletList.size())
				_currentBullet = 0;
			_bulletList.at(_currentBullet)->Shoot(Position, _shotDirection, _type);
			_currentBullet++;
		}
	}
}

bool NPC::CheckPlayerInRange(int x, int y, bool isCollect)
{
	if (_state != NPCS_RunNoAttack)
	{
		//kiểm tra trong tầm bắn
		int posX, posY;
		posX = (Position.x / X_STEP);
		posY = (Position.y / Y_STEP);

		int left, right, top, bottom;

		if (x <= posX + _attackRange * _attackRangeFactor && x >= posX - _attackRange * _attackRangeFactor
			&& y <= posY + _attackRange * _attackRangeFactor && y >= posY - _attackRange * _attackRangeFactor)
		{
			if (rand() % 100 < _attackRatio * _attackRatio)
			{
				_isEnemyInRange = true;
				if (isCollect) _state = NPCS_RunCollect;
				vecPlayer->x = x;
				vecPlayer->y = y;
			}
			else _state = NPCS_RunNoAttack;
		}
		else _isEnemyInRange = false;
		return _isEnemyInRange;
	}
}