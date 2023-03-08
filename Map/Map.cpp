#include "Map.h"

Map::Map()
{
	BlockSize = 32;
}

Map::~Map() {}

void Map::Initialize(Model* model, Model* floorModel) {
	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	BlockSize = 32;
	input_ = Input::GetInstance();
	model_ = model;
	floorModel_ = floorModel;

	debugText_ = DebugText::GetInstance();

	//�J�����̉�p�̒���
	for (int z = 0; z < Map_Z; z++) {
		for (int x = 0; x < Map_X; x++) {
			worldTransform_[z][x].Initialize();
			worldTransform_[z][x].translation_.x = 2.0f * z - 30.0f;
			worldTransform_[z][x].translation_.y = 0.0f;
			worldTransform_[z][x].translation_.z = 2.0f * x - 22.0f;
			myFunc_.UpdateWorldTransform(worldTransform_[z][x]);

			worldTransform_[z][x].TransferColorMatrix();
		}
	}

	floorWorldTransform_.Initialize();
	floorWorldTransform_.translation_ = { 0,-7,0 };
	myFunc_.UpdateWorldTransform(floorWorldTransform_);
	floorWorldTransform_.TransferColorMatrix();
	//�f�o�b�O�p�\��
	debugText_->SetPos(50, 140);
	debugText_->Printf(" pos:(%f, %f, %f)", worldTransform_[0][0].translation_.x, worldTransform_[0][0].translation_.y, worldTransform_[0][0].translation_.z);

	goalFlag = false;
}

void Map::Update(Player* player) {

	//�}�b�v�`�b�v�ƃv���C���[���������Ă��邩�m�F����
	BlockCheck(player);

	//�}�b�v�t���O�̐؂�ւ�
	if (input_->TriggerKey(DIK_L)) {
		if (MapFlag == 1) {
			MapFlag = 0;
		}
	}

	//�����t���O�̐؂�ւ�
	if (input_->TriggerKey(DIK_K)) {
		if (invisibleFlag == true) {
			player->GetTransform() = { -11.0f, 0.0f, -18.0f };
			invisibleFlag = false;
		}
		else if (invisibleFlag == false) {
			player->GetTransform() = { 0.0f, 0.0f, 0.0f };
			invisibleFlag = true;
		}
	}

	if (input_->TriggerKey(DIK_D)) {
		if (AnswerFlag == true) {
			AnswerFlag = false;
		}
		else if (AnswerFlag == false) {
			AnswerFlag = true;
		}
	}

	//�f�o�b�O�p�\��
	debugText_->SetPos(50, 180);
	debugText_->Printf("MapPlayer pos:(%f, %f, %f)", player->GetWorldPosition().x, player->GetWorldPosition().y, player->GetWorldPosition().z);

	debugText_->SetPos(50, 210);
	debugText_->Printf("invisibleFlag:%d", invisibleFlag);

	debugText_->SetPos(50, 230);
	debugText_->Printf("MapPlayer pos:(%f, %f, %f)", player->GetTransform().x, player->GetTransform().y, player->GetTransform().z);

	debugText_->SetPos(50, 320);
	debugText_->Printf("GoalCount:%d", GoalCount);
}

void Map::Draw(ViewProjection& viewProjection) {

	if (MapFlag == 0)
	{
		//3D���f����`��
		for (int z = 0; z < Map_Z; z++) {
			for (int x = 0; x < Map_X; x++) {
				if (invisibleFlag == false) {
					if (FirstMap[z][x] == BLOCK) {
						model_->Draw(worldTransform_[z][x], viewProjection);
					}
				}
				if (FirstMap[z][x] == WALL) {
					model_->Draw(worldTransform_[z][x], viewProjection);
				}
				if (AnswerFlag==true) {
					if (FirstMap[z][x] == GOAL) {
						model_->Draw(worldTransform_[z][x], viewProjection);
					}
				}
			}
		}
	}

	//if (MapFlag == 1)
	//{
	//	//3D���f����`��
	//	for (int z = 0; z < 20; z++) {
	//		for (int x = 0; x < 25; x++) {
	//			if (SecondMap[z][x] == BLOCK) {
	//				model_->Draw(worldTransform_[z][x], viewProjection);
	//			}
	//		}
	//	}
	//}

}

void Map::FloorDraw(ViewProjection& viewProjection) {
	//3D���f����`��
	floorModel_->Draw(floorWorldTransform_, viewProjection);

}


void Map::OnCollision(Vector3 playerPos, float radius) {
	//�E��̃}�b�v�`�b�v��ł̍��W���擾

}

//int Map::BlockCheck(int line, int culumn) {
//	if (MapFlag == 0)
//	{
//		//�}�b�v�i�񂾐�Ƀu���b�N����������1��Ԃ�
//		if (FirstMap[line][culumn] == BLOCK) {
//			return 1;
//		}
//		if (FirstMap[line][culumn] == WALL) {
//			return 1;
//		}
//	}
//	//if (MapFlag == 1)
//	//{
//	//	//�}�b�v�i�񂾐�Ƀu���b�N����������1��Ԃ�
//	//	if (SecondMap[line][culumn] == BLOCK) {
//	//		return 1;
//	//	}
//	//}
//
//	return 0;
//}

void Map::BlockCheck(Player* player) {
	if (MapFlag == 0) {
		for (int z = 0; z < Map_Z; z++) {
			for (int x = 0; x < Map_X; x++) {
				// �u���b�N�̍��W
				worldTransform_[z][x].translation_;

				// �v���C���[�̍��W
				player->GetWorldPosition();

				//�v���C���[�̈ړ�����O�̏ꏊ��ۑ�����ϐ�
				Vector3 oldPlayerPos;

				oldPlayerPos = player->GetprePosition();

				// �u���b�N�̂ǂ��瑤����Ԃ�������
				float blockLeftX = worldTransform_[z][x].translation_.x - radius;
				float blockRightX = worldTransform_[z][x].translation_.x + radius;
				float blockUpZ = worldTransform_[z][x].translation_.z + radius;
				float blockDownZ = worldTransform_[z][x].translation_.z - radius;

				float playerLeftX = oldPlayerPos.x - player->GetRadius();
				float playerRightX = oldPlayerPos.x + player->GetRadius();
				float playerUpZ = oldPlayerPos.z + player->GetRadius();
				float playerDownZ = oldPlayerPos.z - player->GetRadius();

				float distance = 0.1f;

				//�i�񂾐悪BLOCK��������i�܂��Ȃ�����
				if (FirstMap[z][x] == BLOCK) {

					// �v���C���[�ƃu���b�N�Փ˔���
					if (CheckCollision(worldTransform_[z][x].translation_, player->GetWorldPosition(), radius, player->GetRadius())) {

						/*
							�u���b�N���v���C���[�̎l�p�̍��[�A�E�[�A��[�A���[�A���S�_���r���ĂԂ������������ʂ���

							��@������
							�v���C���[�̒��S�_�ƃu���b�N�̒��S�_��ׂăv���C���[�����ɂ���͂�
							�܂����ꂾ�ƍ��A�E����Ԃ����Ă����ꍇ������

							�ړ�����O�̃v���C���[(oldPlayerPos)�ɉE�[�ƃu���b�N�̍��[���ׂăv���C���[�̉E�[���傫�������獶����Ԃ���̂͂��肦�Ȃ�
							�t�̂��ƌ�����A�E����Ԃ���̂����肦�Ȃ����Ƃ�������̂ł��̎O�̏������B������Ă���ꍇ������Ԃ����Ă���
						*/

						// �㉺
						if (playerRightX > blockLeftX && playerLeftX < blockRightX) {
							if (player->GetWorldPosition().z < worldTransform_[z][x].translation_.z) {
								oldPlayerPos.z -= distance;
								player->SetWorldPosition(oldPlayerPos);
							}
							else if (player->GetWorldPosition().z > worldTransform_[z][x].translation_.z) {
								oldPlayerPos.z += distance;
								player->SetWorldPosition(oldPlayerPos);
							}
						}
						// ���E
						else if (playerUpZ > blockDownZ && playerDownZ < blockUpZ) {
							if (player->GetWorldPosition().x < worldTransform_[z][x].translation_.x) {
								oldPlayerPos.x -= distance;
								player->SetWorldPosition(oldPlayerPos);
							}
							else if (player->GetWorldPosition().x > worldTransform_[z][x].translation_.x) {
								oldPlayerPos.x += distance;
								player->SetWorldPosition(oldPlayerPos);
							}
						}

						// ������ 
						if (player->GetWorldPosition().z < worldTransform_[z][x].translation_.z &&
							playerRightX > blockLeftX &&
							playerLeftX < blockRightX) {
							debugText_->SetPos(50, 250);
							debugText_->Printf("down:hit");
						}

						// �ォ��
						if (player->GetWorldPosition().z > worldTransform_[z][x].translation_.z &&
							playerRightX > blockLeftX &&
							playerLeftX < blockRightX) {
							debugText_->SetPos(50, 270);
							debugText_->Printf("up:hit");
						}

						// ������
						if (player->GetWorldPosition().x < worldTransform_[z][x].translation_.x &&
							playerUpZ > blockDownZ &&
							playerDownZ < blockUpZ) {
							debugText_->SetPos(50, 290);
							debugText_->Printf("left:hit");
						}

						// �E����
						if (player->GetWorldPosition().x > worldTransform_[z][x].translation_.x &&
							playerUpZ > blockDownZ &&
							playerDownZ < blockUpZ) {
							debugText_->SetPos(50, 310);
							debugText_->Printf("right:hit");
						}
					}
				}
				//�i�񂾐悪WALL��������i�܂��Ȃ�����
				if (FirstMap[z][x] == WALL) {

					// �v���C���[�ƃu���b�N�Փ˔���
					if (CheckCollision(worldTransform_[z][x].translation_, player->GetWorldPosition(), radius, player->GetRadius())) {

						/*
							�u���b�N���v���C���[�̎l�p�̍��[�A�E�[�A��[�A���[�A���S�_���r���ĂԂ������������ʂ���

							��@������
							�v���C���[�̒��S�_�ƃu���b�N�̒��S�_��ׂăv���C���[�����ɂ���͂�
							�܂����ꂾ�ƍ��A�E����Ԃ����Ă����ꍇ������

							�ړ�����O�̃v���C���[(oldPlayerPos)�ɉE�[�ƃu���b�N�̍��[���ׂăv���C���[�̉E�[���傫�������獶����Ԃ���̂͂��肦�Ȃ�
							�t�̂��ƌ�����A�E����Ԃ���̂����肦�Ȃ����Ƃ�������̂ł��̎O�̏������B������Ă���ꍇ������Ԃ����Ă���
						*/

						// �㉺
						if (playerRightX > blockLeftX && playerLeftX < blockRightX) {
							if (player->GetWorldPosition().z < worldTransform_[z][x].translation_.z) {
								oldPlayerPos.z -= distance;
								player->SetWorldPosition(oldPlayerPos);
							}
							else if (player->GetWorldPosition().z > worldTransform_[z][x].translation_.z) {
								oldPlayerPos.z += distance;
								player->SetWorldPosition(oldPlayerPos);
							}
						}
						// ���E
						else if (playerUpZ > blockDownZ && playerDownZ < blockUpZ) {
							if (player->GetWorldPosition().x < worldTransform_[z][x].translation_.x) {
								oldPlayerPos.x -= distance;
								player->SetWorldPosition(oldPlayerPos);
							}
							else if (player->GetWorldPosition().x > worldTransform_[z][x].translation_.x) {
								oldPlayerPos.x += distance;
								player->SetWorldPosition(oldPlayerPos);
							}
						}

						// ������ 
						if (player->GetWorldPosition().z < worldTransform_[z][x].translation_.z &&
							playerRightX > blockLeftX &&
							playerLeftX < blockRightX) {
							debugText_->SetPos(50, 250);
							debugText_->Printf("down:hit");
						}

						// �ォ��
						if (player->GetWorldPosition().z > worldTransform_[z][x].translation_.z &&
							playerRightX > blockLeftX &&
							playerLeftX < blockRightX) {
							debugText_->SetPos(50, 270);
							debugText_->Printf("up:hit");
						}

						// ������
						if (player->GetWorldPosition().x < worldTransform_[z][x].translation_.x &&
							playerUpZ > blockDownZ &&
							playerDownZ < blockUpZ) {
							debugText_->SetPos(50, 290);
							debugText_->Printf("left:hit");
						}

						// �E����
						if (player->GetWorldPosition().x > worldTransform_[z][x].translation_.x &&
							playerUpZ > blockDownZ &&
							playerDownZ < blockUpZ) {
							debugText_->SetPos(50, 310);
							debugText_->Printf("right:hit");
						}
					}
				}
				//�i�񂾐悪GOAL��������N���A��ʂɈڍs����
				if (FirstMap[z][x] == GOAL) {

					// �v���C���[�ƃu���b�N�Փ˔���
					if (CheckCollision(worldTransform_[z][x].translation_, player->GetWorldPosition(), radius, player->GetRadius())) {

						/*
							�u���b�N���v���C���[�̎l�p�̍��[�A�E�[�A��[�A���[�A���S�_���r���ĂԂ������������ʂ���

							��@������
							�v���C���[�̒��S�_�ƃu���b�N�̒��S�_��ׂăv���C���[�����ɂ���͂�
							�܂����ꂾ�ƍ��A�E����Ԃ����Ă����ꍇ������

							�ړ�����O�̃v���C���[(oldPlayerPos)�ɉE�[�ƃu���b�N�̍��[���ׂăv���C���[�̉E�[���傫�������獶����Ԃ���̂͂��肦�Ȃ�
							�t�̂��ƌ�����A�E����Ԃ���̂����肦�Ȃ����Ƃ�������̂ł��̎O�̏������B������Ă���ꍇ������Ԃ����Ă���
						*/

						// �㉺
						if (playerRightX > blockLeftX && playerLeftX < blockRightX) {
							if (player->GetWorldPosition().z < worldTransform_[z][x].translation_.z) {
								GoalCount++;
								if (GoalCount > 100)
								{
									goalFlag = true;
								}
							}
							else if (player->GetWorldPosition().z > worldTransform_[z][x].translation_.z) {
								GoalCount++;
								if (GoalCount > 100)
								{
									goalFlag = true;
								}
							}
						}
						// ���E
						else if (playerUpZ > blockDownZ && playerDownZ < blockUpZ) {
							if (player->GetWorldPosition().x < worldTransform_[z][x].translation_.x) {
								GoalCount++;
								if (GoalCount > 100)
								{
									goalFlag = true;
								}
							}
							else if (player->GetWorldPosition().x > worldTransform_[z][x].translation_.x) {
								GoalCount++;
								if (GoalCount > 100)
								{
									goalFlag = true;
								}
							}
						}

					}
				}

			}
		}

	}


	//if (MapFlag == 1) {
	//	for (int z = 0; z < 20; z++) {
	//		for (int x = 0; x < 25; x++) {
	//			if (SecondMap[z][x] == BLOCK) {
	//				// �u���b�N�̍��W
	//				worldTransform_[z][x].translation_;

	//				// �v���C���[�̍��W
	//				player->GetWorldPosition();

	//				// �v���C���[�ƃu���b�N�Փ˔���
	//				if (CheckCollision(worldTransform_[z][x].translation_, player->GetWorldPosition(), radius, player->GetRadius())) {
	//					//�v���C���[�̈ړ�����O�̏ꏊ��ۑ�����ϐ�
	//					Vector3 oldPlayerPos;

	//					oldPlayerPos = player->GetprePosition();

	//					// �u���b�N�̂ǂ��瑤����Ԃ�������
	//					float blockLeftX = worldTransform_[z][x].translation_.x - radius;
	//					float blockRightX = worldTransform_[z][x].translation_.x + radius;
	//					float blockUpZ = worldTransform_[z][x].translation_.z + radius;
	//					float blockDownZ = worldTransform_[z][x].translation_.z - radius;

	//					float playerLeftX = oldPlayerPos.x - player->GetRadius();
	//					float playerRightX = oldPlayerPos.x + player->GetRadius();
	//					float playerUpZ = oldPlayerPos.z + player->GetRadius();
	//					float playerDownZ = oldPlayerPos.z - player->GetRadius();

	//					float distance = 0.1f;

	//					/*
	//						�u���b�N���v���C���[�̎l�p�̍��[�A�E�[�A��[�A���[�A���S�_���r���ĂԂ������������ʂ���

	//						��@������
	//						�v���C���[�̒��S�_�ƃu���b�N�̒��S�_��ׂăv���C���[�����ɂ���͂�
	//						�܂����ꂾ�ƍ��A�E����Ԃ����Ă����ꍇ������

	//						�ړ�����O�̃v���C���[(oldPlayerPos)�ɉE�[�ƃu���b�N�̍��[���ׂăv���C���[�̉E�[���傫�������獶����Ԃ���̂͂��肦�Ȃ�
	//						�t�̂��ƌ�����A�E����Ԃ���̂����肦�Ȃ����Ƃ�������̂ł��̎O�̏������B������Ă���ꍇ������Ԃ����Ă���
	//					*/

	//					// �㉺
	//					if (playerRightX > blockLeftX && playerLeftX < blockRightX) {
	//						if (player->GetWorldPosition().z < worldTransform_[z][x].translation_.z) {
	//							oldPlayerPos.z -= distance;
	//							player->SetWorldPosition(oldPlayerPos);
	//						}
	//						else if (player->GetWorldPosition().z > worldTransform_[z][x].translation_.z) {
	//							oldPlayerPos.z += distance;
	//							player->SetWorldPosition(oldPlayerPos);
	//						}
	//					}
	//					// ���E
	//					else if (playerUpZ > blockDownZ && playerDownZ < blockUpZ) {
	//						if (player->GetWorldPosition().x < worldTransform_[z][x].translation_.x) {
	//							oldPlayerPos.x -= distance;
	//							player->SetWorldPosition(oldPlayerPos);
	//						}
	//						else if (player->GetWorldPosition().x > worldTransform_[z][x].translation_.x) {
	//							oldPlayerPos.x += distance;
	//							player->SetWorldPosition(oldPlayerPos);
	//						}
	//					}

	//					// ������ 
	//					if (player->GetWorldPosition().z < worldTransform_[z][x].translation_.z &&
	//						playerRightX > blockLeftX &&
	//						playerLeftX < blockRightX) {
	//						debugText_->SetPos(50, 250);
	//						debugText_->Printf("down:hit");
	//					}

	//					// �ォ��
	//					if (player->GetWorldPosition().z > worldTransform_[z][x].translation_.z &&
	//						playerRightX > blockLeftX &&
	//						playerLeftX < blockRightX) {
	//						debugText_->SetPos(50, 270);
	//						debugText_->Printf("up:hit");
	//					}

	//					// ������
	//					if (player->GetWorldPosition().x < worldTransform_[z][x].translation_.x &&
	//						playerUpZ > blockDownZ &&
	//						playerDownZ < blockUpZ) {
	//						debugText_->SetPos(50, 290);
	//						debugText_->Printf("left:hit");
	//					}

	//					// �E����
	//					if (player->GetWorldPosition().x > worldTransform_[z][x].translation_.x &&
	//						playerUpZ > blockDownZ &&
	//						playerDownZ < blockUpZ) {
	//						debugText_->SetPos(50, 310);
	//						debugText_->Printf("right:hit");
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
}

bool Map::CheckCollision(Vector3 pos1, Vector3 pos2, float radius1, float radius2) {

	if (pos1.x - radius1 < pos2.x + radius2 && pos2.x - radius2 < pos1.x + radius1) {
		if (pos1.z - radius1 < pos2.z + radius2 && pos2.z - radius2 < pos1.z + radius1) {
			return 1;
		}
	}
	return 0;
}

void Map::Reset()
{
	invisibleFlag = false;
	goalFlag = false;
	GoalCount = 0;
}
