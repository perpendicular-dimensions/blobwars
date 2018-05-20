void loadResources();
void loadSprite(std::string_view token);
void loadSprite(const std::string &name, const YAML::Node &data);
void loadSound(int index, const std::string &filename);
