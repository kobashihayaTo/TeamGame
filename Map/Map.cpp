#include "Map.h"

Map::Map()
{
	BlockSize = 32;
}

Map::~Map() {}

void Map::Initialize(Model* model, Model* floorModel, Model* effectmodel_, Model* afterModel) {
	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	BlockSize = 32;
	input_ = Input::GetInstance();
	model_ = model;
	afterModel_ = afterModel;

	assert(effectmodel_);
	effectmodel = effectmodel_;

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
	effectworldTrans.Initialize();
	effectworldTrans.scale_ = { 2.0f,2.0f,2.0f };
	effectworldTrans.translation_ = { -11.0f, 0.0f, -18.0f };
	effectworldTrans.translation_.y -= 10.0f;

}

void Map::Update(Player* player, bool MapkeyFlag) {

	effectworldTrans.translation_.x = player->GetTransform().x;
	effectworldTrans.translation_.z = player->GetTransform().z;

	//�}�b�v�`�b�v�ƃv���C���[���������Ă��邩�m�F����
	PlayerBlockCheck(player);


	//�����t���O�̐؂�ւ�

	if (MapkeyFlag == true) {
		if (AnswerFlag == false) {
			AnswerFlag = true;
		}
	}
	if (AnswerFlag == true) {
		AnswerTimer--;
		if (AnswerTimer < 0) {
			AnswerIntervalFlag = true;
		}
	}
	if (AnswerIntervalFlag == true) {
		AnswerIntervalTimer--;
		if (AnswerIntervalTimer <= 0.01f) {
			OKFlag = true;
		}
		if (AnswerIntervalTimer < 0) {
			OKFlag = false;
			AnswerFlag = false;
			AnswerIntervalFlag = false;
			AnswerTimer = 100;
			AnswerIntervalTimer = 100;
		}
	}


	effectworldTrans.TransferColorMatrix();
	myFunc_.UpdateWorldTransform(effectworldTrans);


	//�f�o�b�O�p�\��

	debugText_->SetPos(50, 150);
	debugText_->Printf("effect pos:(%f, %f, %f)", effectworldTrans.translation_.x, effectworldTrans.translation_.y, effectworldTrans.translation_.z);

	debugText_->SetPos(50, 180);
	debugText_->Printf("MapPlayer pos:(%f, %f, %f)", player->GetWorldPosition().x, player->GetWorldPosition().y, player->GetWorldPosition().z);

	debugText_->SetPos(50, 230);
	debugText_->Printf("MapPlayer pos:(%f, %f, %f)", player->GetTransform().x, player->GetTransform().y, player->GetTransform().z);

	debugText_->SetPos(50, 320);
	debugText_->Printf("GoalCount:%d", GoalCount);


	debugText_->SetPos(50, 210);
	debugText_->Printf("AnswerFlag:%d", AnswerFlag);

	debugText_->SetPos(50, 390);
	debugText_->Printf("AnswerIntervalFlag:%d", AnswerIntervalFlag);

	debugText_->SetPos(50, 410);
	debugText_->Printf("AnswerTimer:%d", AnswerTimer);

	debugText_->SetPos(50, 430);
	debugText_->Printf("AnswerIntervalTimer:%d", AnswerIntervalTimer);

	debugText_->SetPos(50, 610);
	debugText_->Printf("GoalCount:%d", GoalCount);
}

void Map::Draw(ViewProjection& viewProjection) {

	if (MapFlag == 0)
	{
		//3D���f����`��
		for (int z = 0; z < Map_Z; z++) {
			for (int x = 0; x < Map_X; x++) {
				if (FirstMap[z][x] == BLOCK) {
					afterModel_->Draw(worldTransform_[z][x], viewProjection);
				}
				if (FirstMap[z][x] == WALL) {
					model_->Draw(worldTransform_[z][x], viewProjection);
				}
				if (AnswerFlag == true && AnswerIntervalFlag == false) {
					if (FirstMap[z][x] == GOAL) {
						model_->Draw(worldTransform_[z][x], viewProjection);
					}
				}
			}
		}
	}
	effectmodel->Draw(effectworldTrans, viewProjection);
}

void Map::FloorDraw(ViewProjection& viewProjection) {
	//3D���f����`��
	floorModel_->Draw(floorWorldTransform_, viewProjection);

}


void Map::OnCollision(Vector3 playerPos, float radius) {
	//�E��̃}�b�v�`�b�v��ł̍��W���擾

}


void Map::PlayerBlockCheck(Player* player) {

	if (MapFlag == 0) {
		UIFlag = false;
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
					testFlag = CheckCollision(worldTransform_[z][x].translation_, player->GetWorldPosition(), radius, player->GetRadius());
					// �v���C���[�ƃu���b�N�Փ˔���
					if (testFlag == true) {
						GoalCount += 1;
						UIFlag = true;
						if (GoalCount > 100) {
							//UI�̃t���O
							UIFlag = false;
							goalReadyFlag = true;
							GoalCount = 100;
						}
						if (goalReadyFlag == true) {
							effectworldTrans.translation_.y += 0.5f;
							if (effectworldTrans.translation_.y >= 0.0f) {
								effectworldTrans.translation_.y = 0.0f;
								effectOffFlag = true;
								goalReadyFlag = false;

							}
						}
						if (effectOffFlag == true) {
							effectworldTrans.translation_.y -= 1.0f;
							if (effectworldTrans.translation_.y <= -10.0f) {
								effectworldTrans.translation_.y = -10.0f;
								goalcount++;
								if (goalcount > 100) {
									goalFlag = true;
								}
							}
						}
						debugText_->SetPos(50, 710);
						debugText_->Printf("effectworldTrans.translation_.y:%f", effectworldTrans.translation_.y);
						/*
							�u���b�N���v���C���[�̎l�p�̍��[�A�E�[�A��[�A���[�A���S�_���r���ĂԂ������������ʂ���

							��@������
							�v���C���[�̒��S�_�ƃu���b�N�̒��S�_��ׂăv���C���[�����ɂ���͂�
							�܂����ꂾ�ƍ��A�E����Ԃ����Ă����ꍇ������

							�ړ�����O�̃v���C���[(oldPlayerPos)�ɉE�[�ƃu���b�N�̍��[���ׂăv���C���[�̉E�[���傫�������獶����Ԃ���̂͂��肦�Ȃ�
							�t�̂��ƌ�����A�E����Ԃ���̂����肦�Ȃ����Ƃ�������̂ł��̎O�̏������B������Ă���ꍇ������Ԃ����Ă���
						*/
					}
				}
			}
		}
	}
}

void Map::EnemyBlockCheck(Enemy* enemy) {
	if (MapFlag == 0) {
		for (int z = 0; z < Map_Z; z++) {
			for (int x = 0; x < Map_X; x++) {
				// �u���b�N�̍��W
				worldTransform_[z][x].translation_;

				// �v���C���[�̍��W
				enemy->GetWorldPosition();

				//�v���C���[�̈ړ�����O�̏ꏊ��ۑ�����ϐ�
				Vector3 oldPlayerPos;

				oldPlayerPos = enemy->GetprePosition();

				// �u���b�N�̂ǂ��瑤����Ԃ�������
				float blockLeftX = worldTransform_[z][x].translation_.x - radius;
				float blockRightX = worldTransform_[z][x].translation_.x + radius;
				float blockUpZ = worldTransform_[z][x].translation_.z + radius;
				float blockDownZ = worldTransform_[z][x].translation_.z - radius;

				float playerLeftX = oldPlayerPos.x - enemy->GetRadius();
				float playerRightX = oldPlayerPos.x + enemy->GetRadius();
				float playerUpZ = oldPlayerPos.z + enemy->GetRadius();
				float playerDownZ = oldPlayerPos.z - enemy->GetRadius();

				float distance = 0.1f;

				//�i�񂾐悪BLOCK��������i�܂��Ȃ�����
				if (FirstMap[z][x] == BLOCK) {

					// �v���C���[�ƃu���b�N�Փ˔���
					if (CheckCollision(worldTransform_[z][x].translation_, enemy->GetWorldPosition(), radius, enemy->GetRadius())) {

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
							if (enemy->GetWorldPosition().z < worldTransform_[z][x].translation_.z) {
								oldPlayerPos.z -= distance;
								enemy->SetWorldPosition(oldPlayerPos);
							}
							else if (enemy->GetWorldPosition().z > worldTransform_[z][x].translation_.z) {
								oldPlayerPos.z += distance;
								enemy->SetWorldPosition(oldPlayerPos);
							}
						}
						// ���E
						else if (playerUpZ > blockDownZ && playerDownZ < blockUpZ) {
							if (enemy->GetWorldPosition().x < worldTransform_[z][x].translation_.x) {
								oldPlayerPos.x -= distance;
								enemy->SetWorldPosition(oldPlayerPos);
							}
							else if (enemy->GetWorldPosition().x > worldTransform_[z][x].translation_.x) {
								oldPlayerPos.x += distance;
								enemy->SetWorldPosition(oldPlayerPos);
							}
						}

						// ������ 
						if (enemy->GetWorldPosition().z < worldTransform_[z][x].translation_.z &&
							playerRightX > blockLeftX &&
							playerLeftX < blockRightX) {
							debugText_->SetPos(50, 250);
							debugText_->Printf("down:hit");
						}

						// �ォ��
						if (enemy->GetWorldPosition().z > worldTransform_[z][x].translation_.z &&
							playerRightX > blockLeftX &&
							playerLeftX < blockRightX) {
							debugText_->SetPos(50, 270);
							debugText_->Printf("up:hit");
						}

						// ������
						if (enemy->GetWorldPosition().x < worldTransform_[z][x].translation_.x &&
							playerUpZ > blockDownZ &&
							playerDownZ < blockUpZ) {
							debugText_->SetPos(50, 290);
							debugText_->Printf("left:hit");
						}

						// �E����
						if (enemy->GetWorldPosition().x > worldTransform_[z][x].translation_.x &&
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
					if (CheckCollision(worldTransform_[z][x].translation_, enemy->GetWorldPosition(), radius, enemy->GetRadius())) {

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
							if (enemy->GetWorldPosition().z < worldTransform_[z][x].translation_.z) {
								oldPlayerPos.z -= distance;
								enemy->SetWorldPosition(oldPlayerPos);
							}
							else if (enemy->GetWorldPosition().z > worldTransform_[z][x].translation_.z) {
								oldPlayerPos.z += distance;
								enemy->SetWorldPosition(oldPlayerPos);
							}
						}
						// ���E
						else if (playerUpZ > blockDownZ && playerDownZ < blockUpZ) {
							if (enemy->GetWorldPosition().x < worldTransform_[z][x].translation_.x) {
								oldPlayerPos.x -= distance;
								enemy->SetWorldPosition(oldPlayerPos);
							}
							else if (enemy->GetWorldPosition().x > worldTransform_[z][x].translation_.x) {
								oldPlayerPos.x += distance;
								enemy->SetWorldPosition(oldPlayerPos);
							}
						}

					}

				}

			}
		}

	}
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
	goalFlag = false;
	GoalCount = 0;
	goalcount = 0;

	OKFlag = false;
	AnswerFlag = false;
	AnswerIntervalFlag = false;
	AnswerTimer = 100;
	AnswerIntervalTimer = 100;

	effectworldTrans.translation_ = { -11.0f, 0.0f, -18.0f };
	effectworldTrans.translation_.y -= 10.0f;

	effectOffFlag = false;
	goalReadyFlag = false;
}
