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
	for (int z = 0; z < 20; z++) {
		for (int x = 0; x < 25; x++) {
			worldTransform_[z][x].Initialize();
			worldTransform_[z][x].translation_.x = 2.0f * z - 15.0f;
			worldTransform_[z][x].translation_.y = 0.0f;
			worldTransform_[z][x].translation_.z = 2.0f * x - 22.0f;
			myFunc_.UpdateWorldTransform(worldTransform_[z][x]);
		}
	}

	floorWorldTransform_.Initialize();
	floorWorldTransform_.translation_ = { 0,-7,0 };
	myFunc_.UpdateWorldTransform(floorWorldTransform_);

	//�f�o�b�O�p�\��
	debugText_->SetPos(50, 140);
	debugText_->Printf(" pos:(%f, %f, %f)", worldTransform_[0][0].translation_.x, worldTransform_[0][0].translation_.y, worldTransform_[0][0].translation_.z);


}

void Map::Update(Player* player) {
	BlockCheck(player);
	if (input_->TriggerKey(DIK_L)) {
		if (MapFlag == 1) {
			MapFlag = 0;
		}
		//else if (MapFlag == 0) {
		//	MapFlag = 1;
		//}
	}
	if (input_->TriggerKey(DIK_K)) {
		if (invisibleFlag == true) {
			invisibleFlag = false;
		}
		else if (invisibleFlag == false) {
			invisibleFlag = true;
		}
	}

	//�f�o�b�O�p�\��
	debugText_->SetPos(50, 180);
	debugText_->Printf("MapPlayer pos:(%f, %f, %f)", player->GetWorldPosition().x, player->GetWorldPosition().y, player->GetWorldPosition().z);

	debugText_->SetPos(50, 210);
	debugText_->Printf("MapFlag:%d", MapFlag);

}

void Map::Draw(ViewProjection& viewProjection) {

	if (MapFlag == 0)
	{
		//3D���f����`��
		for (int z = 0; z < 20; z++) {
			for (int x = 0; x < 25; x++) {
				if (invisibleFlag == true) {
					if (FirstMap[z][x] == BLOCK) {
						model_->Draw(worldTransform_[z][x], viewProjection);
					}
				}
				if (FirstMap[z][x] == WALL) {
					model_->Draw(worldTransform_[z][x], viewProjection);
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

int Map::BlockCheck(int line, int culumn) {
	if (MapFlag == 0)
	{
		//�}�b�v�i�񂾐�Ƀu���b�N����������1��Ԃ�
		if (FirstMap[line][culumn] == BLOCK) {
			return 1;
		}
		if (FirstMap[line][culumn] == WALL) {
			return 1;
		}
	}
	//if (MapFlag == 1)
	//{
	//	//�}�b�v�i�񂾐�Ƀu���b�N����������1��Ԃ�
	//	if (SecondMap[line][culumn] == BLOCK) {
	//		return 1;
	//	}
	//}

	return 0;
}

void Map::BlockCheck(Player* player) {
	if (MapFlag == 0){
		for (int z = 0; z < 20; z++) {
			for (int x = 0; x < 25; x++) {
				if (FirstMap[z][x] == BLOCK) {
					// �u���b�N�̍��W
					worldTransform_[z][x].translation_;

					// �v���C���[�̍��W
					player->GetWorldPosition();

					// �v���C���[�ƃu���b�N�Փ˔���
					if (CheckCollision(worldTransform_[z][x].translation_, player->GetWorldPosition(), radius, player->GetRadius())) {
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
				if (FirstMap[z][x] == WALL) {
					// �u���b�N�̍��W
					worldTransform_[z][x].translation_;

					// �v���C���[�̍��W
					player->GetWorldPosition();

					// �v���C���[�ƃu���b�N�Փ˔���
					if (CheckCollision(worldTransform_[z][x].translation_, player->GetWorldPosition(), radius, player->GetRadius())) {
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
