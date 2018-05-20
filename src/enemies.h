void loadDefEnemies();
void loadEnemy(const YAML::Node &data);
void doEnemies();
void enemyBulletCollisions(Entity &bullet);
bool hasClearShot(Entity &enemy);
void addEnemy(const std::string &name, int x, int y, int flags);
Entity *getDefinedEnemy(const std::string &name);
