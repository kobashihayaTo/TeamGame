#include "Map.h"

Map::Map()
{
	BlockSize = 32;
}

Map::~Map() {}

void Map::Initialize(Model* model, Model* floorModel) {
	//引数として受け取ったデータをメンバ変数に記録する
	BlockSize = 32;
	input_ = Input::GetInstance();
	model_ = model;
	floorModel_ = floorModel;

	debugText_ = DebugText::GetInstance();

	//カメラの画角の調整
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

	//デバッグ用表示
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

	//デバッグ用表示
	debugText_->SetPos(50, 180);
	debugText_->Printf("MapPlayer pos:(%f, %f, %f)", player->GetWorldPosition().x, player->GetWorldPosition().y, player->GetWorldPosition().z);

	debugText_->SetPos(50, 210);
	debugText_->Printf("MapFlag:%d", MapFlag);

}

void Map::Draw(ViewProjection& viewProjection) {

	if (MapFlag == 0)
	{
		//3Dモデルを描画
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

int Map::BlockCheck(int line, int culumn) {
	if (MapFlag == 0)
	{
		//マップ進んだ先にブロックがあったら1を返す
		if (FirstMap[line][culumn] == BLOCK) {
			return 1;
		}
		if (FirstMap[line][culumn] == WALL) {
			return 1;
		}
	}
	//if (MapFlag == 1)
	//{
	//	//マップ進んだ先にブロックがあったら1を返す
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
					// ブロックの座標
					worldTransform_[z][x].translation_;

					// プレイヤーの座標
					player->GetWorldPosition();

					// プレイヤーとブロック衝突判定
					if (CheckCollision(worldTransform_[z][x].translation_, player->GetWorldPosition(), radius, player->GetRadius())) {
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
				if (FirstMap[z][x] == WALL) {
					// ブロックの座標
					worldTransform_[z][x].translation_;

					// プレイヤーの座標
					player->GetWorldPosition();

					// プレイヤーとブロック衝突判定
					if (CheckCollision(worldTransform_[z][x].translation_, player->GetWorldPosition(), radius, player->GetRadius())) {
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
			}
		}
	}
	//if (MapFlag == 1) {
	//	for (int z = 0; z < 20; z++) {
	//		for (int x = 0; x < 25; x++) {
	//			if (SecondMap[z][x] == BLOCK) {
	//				// ブロックの座標
	//				worldTransform_[z][x].translation_;

	//				// プレイヤーの座標
	//				player->GetWorldPosition();

	//				// プレイヤーとブロック衝突判定
	//				if (CheckCollision(worldTransform_[z][x].translation_, player->GetWorldPosition(), radius, player->GetRadius())) {
	//					//プレイヤーの移動する前の場所を保存する変数
	//					Vector3 oldPlayerPos;

	//					oldPlayerPos = player->GetprePosition();

	//					// ブロックのどちら側からぶつかったか
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
	//						ブロックかプレイヤーの四角の左端、右端、上端、下端、中心点を比較してぶつかった方向判別する

	//						例　下から
	//						プレイヤーの中心点とブロックの中心点比べてプレイヤーが下にいるはず
	//						まだこれだと左、右からぶつかってきた場合がある

	//						移動する前のプレイヤー(oldPlayerPos)に右端とブロックの左端を比べてプレイヤーの右端が大きかったら左からぶつかるのはありえない
	//						逆のこと言える、右からぶつかるのもありえないことが分かるのでその三つの条件が達成されている場合下からぶつかっている
	//					*/

	//					// 上下
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
	//					// 左右
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

	//					// 下から 
	//					if (player->GetWorldPosition().z < worldTransform_[z][x].translation_.z &&
	//						playerRightX > blockLeftX &&
	//						playerLeftX < blockRightX) {
	//						debugText_->SetPos(50, 250);
	//						debugText_->Printf("down:hit");
	//					}

	//					// 上から
	//					if (player->GetWorldPosition().z > worldTransform_[z][x].translation_.z &&
	//						playerRightX > blockLeftX &&
	//						playerLeftX < blockRightX) {
	//						debugText_->SetPos(50, 270);
	//						debugText_->Printf("up:hit");
	//					}

	//					// 左から
	//					if (player->GetWorldPosition().x < worldTransform_[z][x].translation_.x &&
	//						playerUpZ > blockDownZ &&
	//						playerDownZ < blockUpZ) {
	//						debugText_->SetPos(50, 290);
	//						debugText_->Printf("left:hit");
	//					}

	//					// 右から
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
