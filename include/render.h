#ifndef RENDER_H
#define RENDER_H

void clear_framebuffer();
void draw_background();
void draw_fishes(std::vector<Fish>& fishes);
void draw_menu(Menu& menu);
void render_framebuffer();

#endif
