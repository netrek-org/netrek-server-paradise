#define TERRAIN_TYPE(X,Y) terrain_grid[X*TGRID_SIZE + Y].types

extern void generate_terrain();
extern void place_nebula(int num_nebula, int num_seeds, int minalt);
extern void place_asteroids(int altitude);
extern void doTerrainEffects();

