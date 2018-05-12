void loadDefItems();
void doItems();
void showCarriedItems();
bool carryingItem(const std::string &name);
void dropCarriedItems();
void stealCrystal();
void dropHelperItems(int x, int y);
void dropRandomItems(int x, int y);
void addItem(int itemType, const std::string &name, int x, int y, const std::string &spriteName, int health, int value, int flags, bool randomMovement);
