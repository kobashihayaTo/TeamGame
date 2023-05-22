#include "Map.h"

Map::Map()
{
	blockSize = 32;
}

Map::~Map() {}

void Map::Initialize(Model* model, Model* floorModel, Model* effectmodel_, Model* afterModel, Model* goalModel) {
	//引数として受け取ったデータをメンバ変数に記録する
	blockSize = 32;
	input_ = Input::GetInstance();


	assert(effectmodel_);
	model_ = model;
	afterModel_ = afterModel;

	goalModel_ = goalModel;
	effectModel = effectmodel_;

	floorModel_ = floorModel;

	debugText_ = DebugText::GetInstance();

	//カメラの画角の調整
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

	//デバッグ用表示
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

	//マップチップとプレイヤーが当たっているか確認する
	PlayerBlockCheck(player);


	//透明フラグの切り替え

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


	//デバッグ用表示

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
		//3Dモデルを描画
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
		//3Dモデルを描画
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
	//3Dモデルを描画
	floorModel_->Draw(floorWorldTransform_, viewProjection);

}

void Map::OnCollision(Vector3 playerPos, float radius) {
	//右上のマップチップ上での座標を取得

}


void Map::PlayerBlockCheck(Player* player) {

	if (mapFlag == 0) {
		UIFlag = false;
		for (int z = 0; z < map_Z; z++) {
			for (int x = 0; x < map_X; x++) {
				// ブロックの座標
				worldTransform_[z][x].translation_;

				// プレイヤーの座標
				player->GetWorldPosition();

				//プレイヤーの移動する前の場所を保存する変数
				Vector3 oldPlayerPos;

				oldPlayerPos = player->GetprePosition();

				// ブロックのどちら側からぶつかったか
				float blockLeftX = worldTransform_[z][x].translation_.x - radius;
				float blockRightX = worldTransform_[z][x].translation_.x + radius;
				float blockUpZ = worldTransform_[z][x].translation_.z + radius;
				float blockDownZ = worldTransform_[z][x].translation_.z - radius;

				float playerLeftX = oldPlayerPos.x - player->GetRadius();
				float playerRightX = oldPlayerPos.x + player->GetRadius();
				float playerUpZ = oldPlayerPos.z + player->GetRadius();
				float playerDownZ = oldPlayerPos.z - player->GetRadius();

				float distance = 0.1f;

				//進んだ先がBLOCKだったら進ませなくする
				if (FirstMap[z][x] == BLOCK) {

					// プレイヤーとブロック衝突判定
					if (CheckCollision(worldTransform_[z][x].translation_, player->GetWorldPosition(), radius, player->GetRadius())) {

						/*
							ブロックかプレイヤーの四角の左端、右端、上端、下端、中心点を比較してぶつかった方向判別する

							例　下から
							プレイヤーの中心点とブロックの中心点比べてプレイヤーが下にいるはず
							まだこれだと左、右からぶつかってきた場合がある

							移動する前のプレイヤー(oldPlayerPos)に右端とブロックの左端を比べてプレイヤーの右端が大きかったら左からぶつかるのはありえない
							逆のこと言える、右からぶつかるのもありえないことが分かるのでその三つの条件が達成されている場合下からぶつかっている
						*/

						// 上下
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
						// 左右
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

						// 下から 
						if (player->GetWorldPosition().z < worldTransform_[z][x].translation_.z &&
							playerRightX > blockLeftX &&
							playerLeftX < blockRightX) {
							debugText_->SetPos(50, 250);
							debugText_->Printf("down:hit");
						}

						// 上から
						if (player->GetWorldPosition().z > worldTransform_[z][x].translation_.z &&
							playerRightX > blockLeftX &&
							playerLeftX < blockRightX) {
							debugText_->SetPos(50, 270);
							debugText_->Printf("up:hit");
						}

						// 左から
						if (player->GetWorldPosition().x < worldTransform_[z][x].translation_.x &&
							playerUpZ > blockDownZ &&
							playerDownZ < blockUpZ) {
							debugText_->SetPos(50, 290);
							debugText_->Printf("left:hit");
						}

						// 右から
						if (player->GetWorldPosition().x > worldTransform_[z][x].translation_.x &&
							playerUpZ > blockDownZ &&
							playerDownZ < blockUpZ) {
							debugText_->SetPos(50, 310);
							debugText_->Printf("right:hit");
						}
					}
				}
				//進んだ先がWALLだったら進ませなくする
				if (FirstMap[z][x] == WALL) {

					// プレイヤーとブロック衝突判定
					if (CheckCollision(worldTransform_[z][x].translation_, player->GetWorldPosition(), radius, player->GetRadius())) {

						/*
							ブロックかプレイヤーの四角の左端、右端、上端、下端、中心点を比較してぶつかった方向判別する

							例　下から
							プレイヤーの中心点とブロックの中心点比べてプレイヤーが下にいるはず
							まだこれだと左、右からぶつかってきた場合がある

							移動する前のプレイヤー(oldPlayerPos)に右端とブロックの左端を比べてプレイヤーの右端が大きかったら左からぶつかるのはありえない
							逆のこと言える、右からぶつかるのもありえないことが分かるのでその三つの条件が達成されている場合下からぶつかっている
						*/

						// 上下
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
						// 左右
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

						// 下から 
						if (player->GetWorldPosition().z < worldTransform_[z][x].translation_.z &&
							playerRightX > blockLeftX &&
							playerLeftX < blockRightX) {
							debugText_->SetPos(50, 250);
							debugText_->Printf("down:hit");
						}

						// 上から
						if (player->GetWorldPosition().z > worldTransform_[z][x].translation_.z &&
							playerRightX > blockLeftX &&
							playerLeftX < blockRightX) {
							debugText_->SetPos(50, 270);
							debugText_->Printf("up:hit");
						}

						// 左から
						if (player->GetWorldPosition().x < worldTransform_[z][x].translation_.x &&
							playerUpZ > blockDownZ &&
							playerDownZ < blockUpZ) {
							debugText_->SetPos(50, 290);
							debugText_->Printf("left:hit");
						}

						// 右から
						if (player->GetWorldPosition().x > worldTransform_[z][x].translation_.x &&
							playerUpZ > blockDownZ &&
							playerDownZ < blockUpZ) {
							debugText_->SetPos(50, 310);
							debugText_->Printf("right:hit");

						}
					}
				}

				if (goal_ == 0) {
					//進んだ先がGOALだったらクリア画面に移行する
					if (FirstMap[z][x] == MIDDLEGOAL1) {
						testFlag = CheckCollision(worldTransform_[z][x].translation_, player->GetWorldPosition(), radius, player->GetRadius());
						// プレイヤーとブロック衝突判定
						if (testFlag == true) {
							goalMidCount1 -= 0.25f;
							UIFlag = true;
							resetFlag = true;
							if (goalMidCount1 <= 0.0f) {
								testFlag = false;
								//UIのフラグ
								UIFlag = false;
								goal_ = 1;
								goalMidCount1 = 50;
								resetFlag = false;
							}
						}
					}
				}

				if (goal_ == 1) {
					//進んだ先がGOALだったらクリア画面に移行する
					if (FirstMap[z][x] == MIDDLEGOAL2) {
						testFlag = CheckCollision(worldTransform_[z][x].translation_, player->GetWorldPosition(), radius, player->GetRadius());
						// プレイヤーとブロック衝突判定
						if (testFlag == true) {
							goalMidCount2 -= 0.25f;
							UIFlag = true;
							resetFlag = true;
							if (goalMidCount2 <= 0.0f) {
								//UIのフラグ
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
					//進んだ先がGOALだったらクリア画面に移行する
					if (FirstMap[z][x] == GOAL) {
						testFlag = CheckCollision(worldTransform_[z][x].translation_, player->GetWorldPosition(), radius, player->GetRadius());
						// プレイヤーとブロック衝突判定
						if (testFlag == true) {
							goalCount -= 0.25f;
							UIFlag = true;
							resetFlag = true;
							if (goalCount <= 0.0f) {
								//UIのフラグ
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
								ブロックかプレイヤーの四角の左端、右端、上端、下端、中心点を比較してぶつかった方向判別する

								例　下から
								プレイヤーの中心点とブロックの中心点比べてプレイヤーが下にいるはず
								まだこれだと左、右からぶつかってきた場合がある

								移動する前のプレイヤー(oldPlayerPos)に右端とブロックの左端を比べてプレイヤーの右端が大きかったら左からぶつかるのはありえない
								逆のこと言える、右からぶつかるのもありえないことが分かるのでその三つの条件が達成されている場合下からぶつかっている
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
				// ブロックの座標
				worldTransform_[z][x].translation_;

				// プレイヤーの座標
				enemy->GetWorldPosition();

				//プレイヤーの移動する前の場所を保存する変数
				Vector3 oldPlayerPos;

				oldPlayerPos = enemy->GetprePosition();

				// ブロックのどちら側からぶつかったか
				float blockLeftX = worldTransform_[z][x].translation_.x - radius;
				float blockRightX = worldTransform_[z][x].translation_.x + radius;
				float blockUpZ = worldTransform_[z][x].translation_.z + radius;
				float blockDownZ = worldTransform_[z][x].translation_.z - radius;

				float playerLeftX = oldPlayerPos.x - enemy->GetRadius();
				float playerRightX = oldPlayerPos.x + enemy->GetRadius();
				float playerUpZ = oldPlayerPos.z + enemy->GetRadius();
				float playerDownZ = oldPlayerPos.z - enemy->GetRadius();

				float distance = 0.1f;

				//進んだ先がBLOCKだったら進ませなくする
				if (FirstMap[z][x] == BLOCK) {

					// プレイヤーとブロック衝突判定
					if (CheckCollision(worldTransform_[z][x].translation_, enemy->GetWorldPosition(), radius, enemy->GetRadius())) {

						/*
							ブロックかプレイヤーの四角の左端、右端、上端、下端、中心点を比較してぶつかった方向判別する

							例　下から
							プレイヤーの中心点とブロックの中心点比べてプレイヤーが下にいるはず
							まだこれだと左、右からぶつかってきた場合がある

							移動する前のプレイヤー(oldPlayerPos)に右端とブロックの左端を比べてプレイヤーの右端が大きかったら左からぶつかるのはありえない
							逆のこと言える、右からぶつかるのもありえないことが分かるのでその三つの条件が達成されている場合下からぶつかっている
						*/

						// 上下
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
						// 左右
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

						// 下から 
						if (enemy->GetWorldPosition().z < worldTransform_[z][x].translation_.z &&
							playerRightX > blockLeftX &&
							playerLeftX < blockRightX) {
							debugText_->SetPos(50, 250);
							debugText_->Printf("down:hit");
						}

						// 上から
						if (enemy->GetWorldPosition().z > worldTransform_[z][x].translation_.z &&
							playerRightX > blockLeftX &&
							playerLeftX < blockRightX) {
							debugText_->SetPos(50, 270);
							debugText_->Printf("up:hit");
						}

						// 左から
						if (enemy->GetWorldPosition().x < worldTransform_[z][x].translation_.x &&
							playerUpZ > blockDownZ &&
							playerDownZ < blockUpZ) {
							debugText_->SetPos(50, 290);
							debugText_->Printf("left:hit");
						}

						// 右から
						if (enemy->GetWorldPosition().x > worldTransform_[z][x].translation_.x &&
							playerUpZ > blockDownZ &&
							playerDownZ < blockUpZ) {
							debugText_->SetPos(50, 310);
							debugText_->Printf("right:hit");
						}
					}
				}
				//進んだ先がWALLだったら進ませなくする
				if (FirstMap[z][x] == WALL) {

					// プレイヤーとブロック衝突判定
					if (CheckCollision(worldTransform_[z][x].translation_, enemy->GetWorldPosition(), radius, enemy->GetRadius())) {

						/*
							ブロックかプレイヤーの四角の左端、右端、上端、下端、中心点を比較してぶつかった方向判別する

							例　下から
							プレイヤーの中心点とブロックの中心点比べてプレイヤーが下にいるはず
							まだこれだと左、右からぶつかってきた場合がある

							移動する前のプレイヤー(oldPlayerPos)に右端とブロックの左端を比べてプレイヤーの右端が大きかったら左からぶつかるのはありえない
							逆のこと言える、右からぶつかるのもありえないことが分かるのでその三つの条件が達成されている場合下からぶつかっている
						*/

						// 上下
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
						// 左右
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
