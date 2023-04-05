#include "Map.h"

Map::Map()
{
	BlockSize = 32;
}

Map::~Map() {}

void Map::Initialize(Model* model, Model* floorModel, Model* effectmodel_) {
	//引数として受け取ったデータをメンバ変数に記録する
	BlockSize = 32;
	input_ = Input::GetInstance();
	model_ = model;

	assert(effectmodel_);
	effectmodel = effectmodel_;

	floorModel_ = floorModel;

	debugText_ = DebugText::GetInstance();

	//カメラの画角の調整
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
	//デバッグ用表示
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

	//マップチップとプレイヤーが当たっているか確認する
	PlayerBlockCheck(player);

	//マップフラグの切り替え
	if (input_->TriggerKey(DIK_L)) {
		if (MapFlag == 1) {
			MapFlag = 0;
		}
	}

	//透明フラグの切り替え

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
	

	//デバッグ用表示

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
	debugText_->Printf("playerTimer:%d", playerTimer);
}

void Map::Draw(ViewProjection& viewProjection) {

	if (MapFlag == 0)
	{
		//3Dモデルを描画
		for (int z = 0; z < Map_Z; z++) {
			for (int x = 0; x < Map_X; x++) {
				if (FirstMap[z][x] == BLOCK) {
					model_->Draw(worldTransform_[z][x], viewProjection);
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
	//if (MapFlag == 1)
	//{
	//	//3Dモデルを描画
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
	//3Dモデルを描画
	floorModel_->Draw(floorWorldTransform_, viewProjection);

}


void Map::OnCollision(Vector3 playerPos, float radius) {
	//右上のマップチップ上での座標を取得

}


void Map::PlayerBlockCheck(Player* player) {
	if (MapFlag == 0) {
		for (int z = 0; z < Map_Z; z++) {
			for (int x = 0; x < Map_X; x++) {
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
				//進んだ先がGOALだったらクリア画面に移行する
				if (FirstMap[z][x] == GOAL) {

					// プレイヤーとブロック衝突判定
					if (CheckCollision(worldTransform_[z][x].translation_, player->GetWorldPosition(), radius, player->GetRadius())) {
						GoalCount++;
						if (GoalCount >= 100) {
							goalReadyFlag = true;
						}
						if (goalReadyFlag == true) {
							effectworldTrans.translation_.y += 0.05f;
							if (effectworldTrans.translation_.y >= 0.0f) {
								effectworldTrans.translation_.y = 0.0f;
								goalcount++;
								if (goalcount > 100) {
									goalFlag = true;
								}
								
							}
						}


						//if (GoalCount >= 120) {

						//	/*
						//		ブロックかプレイヤーの四角の左端、右端、上端、下端、中心点を比較してぶつかった方向判別する

						//		例　下から
						//		プレイヤーの中心点とブロックの中心点比べてプレイヤーが下にいるはず
						//		まだこれだと左、右からぶつかってきた場合がある

						//		移動する前のプレイヤー(oldPlayerPos)に右端とブロックの左端を比べてプレイヤーの右端が大きかったら左からぶつかるのはありえない
						//		逆のこと言える、右からぶつかるのもありえないことが分かるのでその三つの条件が達成されている場合下からぶつかっている
						//	*/

						//	// 上下
						//	if (playerRightX > blockLeftX && playerLeftX < blockRightX) {
						//		if (player->GetWorldPosition().z < worldTransform_[z][x].translation_.z) {
						//			goalFlag = true;
						//		}
						//		else if (player->GetWorldPosition().z > worldTransform_[z][x].translation_.z) {
						//			goalFlag = true;
						//		}
						//	}
						//	// 左右
						//	else if (playerUpZ > blockDownZ && playerDownZ < blockUpZ) {
						//		if (player->GetWorldPosition().x < worldTransform_[z][x].translation_.x) {
						//			goalFlag = true;
						//		}
						//		else if (player->GetWorldPosition().x > worldTransform_[z][x].translation_.x) {
						//			goalFlag = true;
						//		}
						//	}
						//}

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
	goalFlag = false;
	GoalCount = 0;

	AnswerFlag = false;
	AnswerIntervalFlag = false;
	AnswerTimer = 100;
	AnswerIntervalTimer = 100;

	effectworldTrans.translation_ = { -11.0f, 0.0f, -18.0f };
	effectworldTrans.translation_.y -= 10.0f;
}

void Map::FlagReset()
{
	AnswerFlag = false;
	AnswerIntervalFlag = false;
	AnswerTimer = 100;
	AnswerIntervalTimer = 100;

	effectworldTrans.translation_ = { -11.0f, 0.0f, -18.0f };
	effectworldTrans.translation_.y -= 10.0f;
}
