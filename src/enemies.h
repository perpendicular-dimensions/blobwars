void loadDefEnemies();
void loadEnemy(const char *token);
void doEnemies();
void enemyBulletCollisions(Entity *bullet);
bool hasClearShot(Entity *enemy);
void addEnemy(const char *name,int x,int y,int flags);
Entity *getDefinedEnemy(const char *name);
