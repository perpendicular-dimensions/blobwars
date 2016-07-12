void loadDefItems();
void doItems();
void showCarriedItems();
bool carryingItem(const char *name);
void dropCarriedItems();
void stealCrystal();
void dropHelperItems(int x,int y);
void dropRandomItems(int x,int y);
void addItem(int itemType,const char *name,int x,int y,const char *spriteName,int health,int value,int flags,bool randomMovement);
