#include "Map.h"

Map::Map()
{
	blockSize = 32;
}

Map::~Map() {}

void Map::Initialize(Model* model, Model* floorModel, Model* effectmodel_, Model* afterModel, Model* goalModel) {
	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	blockSize = 32;
	input_ = Input::GetInstance();


	assert(effectmodel_);
	model_ = model;
	afterModel_ = afterModel;

	goalModel_ = goalModel;
	effectModel = effectmodel_;

	floorModel_ = floorModel;

	debugText_ = DebugText::GetInstance();

	//�J�����̉�p�̒���
	for (int z = 0; z < map_Z; z++) {
		for (int x = 0; x < map_X; x++) {
			worldTransform_[z][x].Initialize();
			worldTransform_[z][x].translation_.x = 2.0f * z - 40.0f;
			worldTransform_[z][x].translation_.y = 0.0f;
			worldTransform_[z][x].translation_.z = 2.0f * x - 22.0f;
			myFunc_.UpdateWorldTransform(worldTransform_[z][x]);

			worldTransform_[z][x].TransferColorMatrix();
		}
	}

	floorWorldTransform_.Initialize();
	floorWorldTransform_.translation_ = { 0.0f, -5.0f, 3.0f };
	//floorWorldTransform_.scale_ = { 42.0f, 1.0f, 25.0f };
	myFunc_.UpdateWorldTransform(floorWorldTransform_);
	floorWorldTransform_.TransferColorMatrix();

	//�f�o�b�O�p�\��
	debugText_->SetPos(50, 140);
	debugText_->Printf(" pos:(%f, %f, %f)", worldTransform_[0][0].translation_.x, worldTransform_[0][0].translation_.y, worldTransform_[0][0].translation_.z);

	goalFlag = false;
	effectWorldTrans.Initialize();
	effectWorldTrans.scale_ = { 2.0f,2.0f,2.0f };
	effectWorldTrans.translation_ = { -11.0f, 0.0f, -18.0f };
	effectWorldTrans.translation_.y -= 10.0f;

}

void Map::Update(Player* player, bool MapkeyFlag) {

	effectWorldTrans.translation_.x = player->GetTransform().x;
	effectWorldTrans.translation_.z = player->GetTransform().z;

	//�}�b�v�`�b�v�ƃv���C���[���������Ă��邩�m�F����
	PlayerBlockCheck(player);


	//�����t���O�̐؂�ւ�

	if (MapkeyFlag == true) {
		if (answerFlag == false) {
			answerFlag = true;
		}
	}
	if (answerFlag == true) {
		answerTimer--;
		if (answerTimer < 0) {
			answerIntervalFlag = true;
		}
	}
	if (answerIntervalFlag == true) {
		answerIntervalTimer--;
		if (answerIntervalTimer <= 0.01f) {
			OKFlag = true;
		}
		if (answerIntervalTimer < 0) {
			OKFlag = false;
			answerFlag = false;
			answerIntervalFlag = false;
			answerTimer = 100;
			answerIntervalTimer = 100;
		}
	}


	effectWorldTrans.TransferColorMatrix();
	myFunc_.UpdateWorldTransform(effectWorldTrans);


	//�f�o�b�O�p�\��

	debugText_->SetPos(50, 150);
	debugText_->Printf("effect pos:(%f, %f, %f)", effectWorldTrans.translation_.x, effectWorldTrans.translation_.y, effectWorldTrans.translation_.z);

	debugText_->SetPos(50, 180);
	debugText_->Printf("MapPlayer pos:(%f, %f, %f)", player->GetWorldPosition().x, player->GetWorldPosition().y, player->GetWorldPosition().z);

	debugText_->SetPos(50, 230);
	debugText_->Printf("MapPlayer pos:(%f, %f, %f)", player->GetTransform().x, player->GetTransform().y, player->GetTransform().z);

	debugText_->SetPos(50, 320);
	debugText_->Printf("GoalCount:%d", goalCount);


	debugText_->SetPos(50, 210);
	debugText_->Printf("AnswerFlag:%d", answerFlag);

	debugText_->SetPos(50, 390);
	debugText_->Printf("AnswerIntervalFlag:%d", answerIntervalFlag);

	debugText_->SetPos(50, 410);
	debugText_->Printf("AnswerTimer:%d", answerTimer);

	debugText_->SetPos(50, 430);
	debugText_->Printf("AnswerIntervalTimer:%d", answerIntervalTimer);

	debugText_->SetPos(50, 610);
	debugText_->Printf("GoalRELAYCount:%d", goalMidCount1);
}

void Map::Draw(ViewProjection& viewProjection) {

	if (mapFlag == 0)
	{
		//3D���f����`��
		for (int z = 0; z < map_Z; z++) {
			for (int x = 0; x < map_X; x++) {
				/*if (FirstMap[z][x] == BLOCK) {
					afterModel_->Draw(worldTransform_[z][x], viewProjection);
				}*/
				if (FirstMap[z][x] == WALL) {
					model_->Draw(worldTransform_[z][x], viewProjection);
				}
				if (answerFlag == true && answerIntervalFlag == false) {

					if (goal_ == 0) {
						if (FirstMap[z][x] == MIDDLEGOAL1) {
							goalModel_->Draw(worldTransform_[z][x], viewProjection);
						}
					}

					if (goal_ == 1) {
						if (FirstMap[z][x] == MIDDLEGOAL2) {
							goalModel_->Draw(worldTransform_[z][x], viewProjection);
						}
					}

					if (goal_ == 2) {
						if (FirstMap[z][x] == GOAL) {
							goalModel_->Draw(worldTransform_[z][x], viewProjection);
						}
					}
				}
			}
		}
	}
	effectModel->Draw(effectWorldTrans, viewProjection);
}

void Map::WallDraw(ViewProjection& viewProjection)
{
	if (mapFlag == 0)
	{
		//3D���f����`��
		for (int z = 0; z < map_Z; z++) {
			for (int x = 0; x < map_X; x++) {
				if (FirstMap[z][x] == BLOCK) {
					afterModel_->Draw(worldTransform_[z][x], viewProjection);
				}
			}
		}
	}
}

void Map::FloorDraw(ViewProjection& viewProjection) {
	//3D���f����`��
	floorModel_->Draw(floorWorldTransform_, viewProjection);

}

void Map::OnCollision(Vector3 playerPos, float radius) {
	//�E��̃}�b�v�`�b�v��ł̍��W���擾

}


void Map::PlayerBlockCheck(Player* player) {

	if (mapFlag == 0) {
		UIFlag = false;
		for (int z = 0; z < map_Z; z++) {
			for (int x = 0; x < map_X; x++) {
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

				if (goal_ == 0) {
					//�i�񂾐悪GOAL��������N���A��ʂɈڍs����
					if (FirstMap[z][x] == MIDDLEGOAL1) {
						testFlag = CheckCollision(worldTransform_[z][x].translation_, player->GetWorldPosition(), radius, player->GetRadius());
						// �v���C���[�ƃu���b�N�Փ˔���
						if (testFlag == true) {
							goalMidCount1 -= 0.25f;
							UIFlag = true;
							resetFlag = true;
							if (goalMidCount1 <= 0.0f) {
								testFlag = false;
								//UI�̃t���O
								UIFlag = false;
								goal_ = 1;
								goalMidCount1 = 50;
								resetFlag = false;
							}
						}
					}
				}

				if (goal_ == 1) {
					//�i�񂾐悪GOAL��������N���A��ʂɈڍs����
					if (FirstMap[z][x] == MIDDLEGOAL2) {
						testFlag = CheckCollision(worldTransform_[z][x].translation_, player->GetWorldPosition(), radius, player->GetRadius());
						// �v���C���[�ƃu���b�N�Փ˔���
						if (testFlag == true) {
							goalMidCount2 -= 0.25f;
							UIFlag = true;
							resetFlag = true;
							if (goalMidCount2 <= 0.0f) {
								//UI�̃t���O
								UIFlag = false;
								goalMidCount2 = 50;
								goal_ = 2;
								testFlag = false;
								resetFlag = false;
							}
						}
					}
				}

				if (goal_ == 2) {
					//�i�񂾐悪GOAL��������N���A��ʂɈڍs����
					if (FirstMap[z][x] == GOAL) {
						testFlag = CheckCollision(worldTransform_[z][x].translation_, player->GetWorldPosition(), radius, player->GetRadius());
						// �v���C���[�ƃu���b�N�Փ˔���
						if (testFlag == true) {
							goalCount -= 0.25f;
							UIFlag = true;
							resetFlag = true;
							if (goalCount <= 0.0f) {
								//UI�̃t���O
								UIFlag = false;
								goalReadyFlag = true;
								goalCount = 50;
								resetFlag = false;
							}
							if (goalReadyFlag == true) {
								effectWorldTrans.translation_.y += 0.5f;
								if (effectWorldTrans.translation_.y >= 0.0f) {
									effectWorldTrans.translation_.y = 0.0f;
									effectOffFlag = true;
									goalReadyFlag = false;

								}
							}
							if (effectOffFlag == true) {
								effectWorldTrans.translation_.y -= 1.0f;
								if (effectWorldTrans.translation_.y <= -10.0f) {
									effectWorldTrans.translation_.y = -10.0f;
									goalCounter++;
									if (goalCounter > 50) {
										goalFlag = true;
									}
								}
							}
							debugText_->SetPos(50, 710);
							debugText_->Printf("effectworldTrans.translation_.y:%f", effectWorldTrans.translation_.y);
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
}

void Map::EnemyBlockCheck(Enemy* enemy) {
	if (mapFlag == 0) {
		for (int z = 0; z < map_Z; z++) {
			for (int x = 0; x < map_X; x++) {
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
	answerFlag = false;
	answerIntervalFlag = false;
	answerTimer = 100;
	answerIntervalTimer = 100;

	goalFlag = false;
	goalReadyFlag = false;
	//
	goalCount = 50;
	goalMidCount1 = 50;
	goalMidCount2 = 50;

	goalCounter = 0;

	OKFlag = false;
	effectOffFlag = false;

	playerTimer = 0;

	UIFlag = false;

	testFlag = false;

	goal_ = 0;

	resetFlag = false;
}
