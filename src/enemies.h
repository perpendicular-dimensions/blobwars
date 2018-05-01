void loadDefEnemies();
void loadEnemy(const std::string &token);
void doEnemies();
void enemyBulletCollisions(Entity &bullet);
bool hasClearShot(Entity &enemy);
void addEnemy(const std::string &name,int x,int y,int flags);
Entity *getDefinedEnemy(const std::string &name);
