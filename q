[1mdiff --git a/universe.cpp b/universe.cpp[m
[1mindex 62dd9d1..e011f8c 100644[m
[1m--- a/universe.cpp[m
[1m+++ b/universe.cpp[m
[36m@@ -98,8 +98,6 @@[m [mvoid Universe::check_for_collisions_and_combine(){[m
             if(d2 < pow(bodies.at(i).radius + bodies.at(j).radius, 2)){[m
                 // Collision![m
 [m
[31m-                printf("Collision!\n");[m
[31m-[m
                 // Combine the bodies[m
                 Vec2f mi = Vec2f(bodies.at(i).mass, bodies.at(i).mass);[m
                 Vec2f mj = Vec2f(bodies.at(j).mass, bodies.at(j).mass);[m
[36m@@ -228,7 +226,7 @@[m [mbool Universe::handle_input(){[m
 [m
 void Universe::init_random_bodies(){[m
 [m
[31m-    uint32_t seed = 825;[m
[32m+[m[32m    uint32_t seed = 8125;[m
     srand(seed);[m
 [m
     // std::cout << "---------------------------------\n";[m
[36m@@ -245,8 +243,8 @@[m [mvoid Universe::init_random_bodies(){[m
     const double size_fy = screenHeight / size_frequency;[m
 [m
     double speed_frequency = 20;[m
[31m-    double speed_octaves = 3;[m
[31m-    const siv::PerlinNoise speed_perlin(seed);[m
[32m+[m[32m    double speed_octaves = 4;[m
[32m+[m[32m    const siv::PerlinNoise speed_perlin(seed + 5);[m
     const double speed_fx = screenWidth / speed_frequency;[m
     const double speed_fy = screenHeight / speed_frequency;[m
 [m
[36m@@ -268,8 +266,8 @@[m [mvoid Universe::init_random_bodies(){[m
                 radius,[m
                 Vec2f(x, y));[m
 [m
[31m-            // double speed_randomizer = speed_perlin.normalizedOctaveNoise2D(x / speed_fx, y / speed_fy, speed_octaves);[m
[31m-            double speed_randomizer = (rand()%200)/100.0 - 1.0;[m
[32m+[m[32m            double speed_randomizer = speed_perlin.normalizedOctaveNoise2D(x / speed_fx, y / speed_fy, speed_octaves) / 0.1;[m
[32m+[m[32m            // double speed_randomizer = (rand()%200)/100.0 - 1.0;[m
             body.speed = Vec2f([m
                 sin(speed_randomizer * M_PI * 2) * size_randomizer * speed_multiplier,[m
                 cos(speed_randomizer * M_PI * 2) * size_randomizer * speed_multiplier[m
