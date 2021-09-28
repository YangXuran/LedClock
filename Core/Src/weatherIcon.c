#include "fonts.h"

const char *weatherIconStr[] = {
"100","101","102","103","104","150","151","152","153","300",
"301","302","303","304","305","306","307","308","309","310",
"311","312","313","314","315","316","317","318","350","351",
"399","400","401","402","403","404","405","406","407","408",
"409","410","456","457",};

const rgbPoint_u weatherIcon[] = {
/* 100 */
{0x000000}, {0x000000}, {0x94FC00}, {0x94FC00}, {0x94FC00}, {0x000000}, {0x000000}, 
{0x000000}, {0x94FC00}, {0x94FC00}, {0x94FC00}, {0x94FC00}, {0x94FC00}, {0x000000}, 
{0x94FC00}, {0x94FC00}, {0x94FC00}, {0x94FC00}, {0x94FC00}, {0x94FC00}, {0x94FC00}, 
{0x94FC00}, {0x94FC00}, {0x94FC00}, {0x94FC00}, {0x94FC00}, {0x94FC00}, {0x94FC00}, 
{0x94FC00}, {0x94FC00}, {0x94FC00}, {0x94FC00}, {0x94FC00}, {0x94FC00}, {0x94FC00}, 
{0x000000}, {0x94FC00}, {0x94FC00}, {0x94FC00}, {0x94FC00}, {0x94FC00}, {0x000000}, 
{0x000000}, {0x000000}, {0x94FC00}, {0x94FC00}, {0x94FC00}, {0x000000}, {0x000000}, 
/* 101 */
{0x000000}, {0x629815}, {0x82CC19}, {0x629815}, {0x000000}, {0x000000}, {0x000000}, 
{0x629815}, {0x82CC19}, {0x82CC19}, {0x82CC19}, {0x629815}, {0x000000}, {0x000000}, 
{0x629815}, {0x82CC19}, {0x82CC19}, {0x82CC19}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0xAEAEAE}, {0x82CC19}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0x5E5E5E}, 
{0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, 
{0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
/* 102 */
{0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x629815}, {0x629815}, {0x629815}, {0x000000}, {0x000000}, {0x000000}, 
{0x629815}, {0x82CC19}, {0x82CC19}, {0x82CC19}, {0x629815}, {0x000000}, {0x000000}, 
{0x629815}, {0x82CC19}, {0x82CC19}, {0x82CC19}, {0xAEAEAE}, {0x848484}, {0x000000}, 
{0x629815}, {0x82CC19}, {0x82CC19}, {0x82CC19}, {0xAEAEAE}, {0xAEAEAE}, {0x848484}, 
{0x000000}, {0x629815}, {0x629815}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, 
{0x000000}, {0x000000}, {0x000000}, {0x6A6A6A}, {0x6A6A6A}, {0x6A6A6A}, {0x000000}, 
/* 103 */
{0x000000}, {0x629815}, {0x629815}, {0x629815}, {0x000000}, {0x000000}, {0x000000}, 
{0x629815}, {0x82CC19}, {0x82CC19}, {0x82CC19}, {0x629815}, {0x000000}, {0x000000}, 
{0x629815}, {0x82CC19}, {0x82CC19}, {0x82CC19}, {0x629815}, {0x000000}, {0x000000}, 
{0x629815}, {0x82CC19}, {0x82CC19}, {0xAEAEAE}, {0xAEAEAE}, {0x848484}, {0x000000}, 
{0x000000}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0x848484}, 
{0x848484}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0x848484}, 
{0x000000}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x000000}, 
/* 104 */
{0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x000000}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x000000}, 
{0x000000}, {0x848484}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0x848484}, 
{0x000000}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0xAEAEAE}, {0xAEAEAE}, 
{0x848484}, {0x848484}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0x848484}, {0x848484}, 
{0x848484}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0xAEAEAE}, {0x848484}, 
{0x000000}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x000000}, 
/* 150 */
{0x000000}, {0x000000}, {0xAFFC18}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0xAFFC18}, {0xAFFC18}, {0x6B9430}, {0x000000}, {0x000000}, {0x000000}, 
{0x6B9430}, {0xAFFC18}, {0xAFFC18}, {0xAFFC18}, {0x000000}, {0x000000}, {0x000000}, 
{0x6B9430}, {0xAFFC18}, {0xAFFC18}, {0xAFFC18}, {0x6B9430}, {0x000000}, {0x000000}, 
{0x6B9430}, {0xAFFC18}, {0xAFFC18}, {0xAFFC18}, {0xAFFC18}, {0x6B9430}, {0x000000}, 
{0x000000}, {0xAFFC18}, {0xAFFC18}, {0xAFFC18}, {0xAFFC18}, {0xAFFC18}, {0xAFFC18}, 
{0x000000}, {0x000000}, {0x6B9430}, {0xAFFC18}, {0xAFFC18}, {0xAFFC18}, {0x162304}, 
/* 151 */
{0x000000}, {0x000000}, {0x000000}, {0x517024}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x000000}, {0x517024}, {0x70B015}, {0x517024}, {0x000000}, {0x000000}, 
{0x000000}, {0x000000}, {0x5E5E5E}, {0x70B015}, {0x70B015}, {0x517024}, {0x000000}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x70B015}, {0x70B015}, {0x517024}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
/* 152 */
{0x000000}, {0x000000}, {0x517024}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x517024}, {0x70B015}, {0x517024}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x517024}, {0x70B015}, {0x70B015}, {0x517024}, {0x000000}, {0x000000}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x70B015}, {0x70B015}, {0x517024}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x70B015}, {0x70B015}, {0x517024}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x517024}, {0x000000}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, {0x000000}, {0x000000}, 
/* 153 */
{0x000000}, {0x000000}, {0x517024}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x517024}, {0x70B015}, {0x517024}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x517024}, {0x70B015}, {0x70B015}, {0x517024}, {0x000000}, {0x000000}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x70B015}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
/* 300 */
{0x517024}, {0x70B015}, {0x70B015}, {0x517024}, {0x000000}, {0x000000}, {0x000000}, 
{0x70B015}, {0x70B015}, {0x70B015}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x517024}, {0x70B015}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x280146}, {0x000000}, {0x000000}, {0x280146}, {0x000000}, {0x000000}, 
{0x000000}, {0x000000}, {0x57009A}, {0x000000}, {0x000000}, {0x57009A}, {0x000000}, 
/* 301 */
{0x517024}, {0x70B015}, {0x70B015}, {0x517024}, {0x000000}, {0x000000}, {0x000000}, 
{0x70B015}, {0x70B015}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x517024}, {0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x280146}, {0x000000}, {0x280146}, {0x000000}, {0x57009A}, {0x000000}, 
{0x000000}, {0x000000}, {0x57009A}, {0x000000}, {0x57009A}, {0x000000}, {0x000000}, 
/* 302 */
{0x000000}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x000000}, {0x000000}, {0x000000}, {0xAEBA49}, {0x000000}, {0x280146}, {0x000000}, 
{0x000000}, {0x280146}, {0x000000}, {0xAEBA49}, {0xAEBA49}, {0x57009A}, {0x000000}, 
{0x000000}, {0x000000}, {0x57009A}, {0x000000}, {0xAEBA49}, {0x000000}, {0x000000}, 
/* 303 */
{0x000000}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x280146}, {0x280146}, {0x000000}, {0xAEBA49}, {0x000000}, {0x280146}, {0x57009A}, 
{0x57009A}, {0x280146}, {0x000000}, {0xAEBA49}, {0xAEBA49}, {0x280146}, {0x000000}, 
{0x000000}, {0x000000}, {0x57009A}, {0x000000}, {0xAEBA49}, {0x000000}, {0x57009A}, 
/* 304 */
{0x000000}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x000000}, {0x280146}, {0x000000}, {0xAEBA49}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x57009A}, {0x000000}, {0xAEBA49}, {0xAEBA49}, {0x280146}, {0x000000}, 
{0x000000}, {0x000000}, {0x000000}, {0x000000}, {0xAEBA49}, {0x000000}, {0x000000}, 
/* 305 */
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x5E5E5E}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x000000}, {0x57009A}, {0x000000}, {0x57009A}, {0x000000}, {0x000000}, 
/* 306 */
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x5E5E5E}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x000000}, {0x57009A}, {0x000000}, {0x000000}, {0x000000}, {0x57009A}, {0x000000}, 
{0x000000}, {0x000000}, {0x000000}, {0x57009A}, {0x000000}, {0x000000}, {0x000000}, 
/* 307 */
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x5E5E5E}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x000000}, {0x57009A}, {0x000000}, {0x280146}, {0x000000}, {0x000000}, {0x57009A}, 
{0x57009A}, {0x000000}, {0x000000}, {0x57009A}, {0x000000}, {0x57009A}, {0x000000}, 
/* 308 */
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x5E5E5E}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x140123}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x140123}, {0x57009A}, {0x000000}, {0x280146}, {0x000000}, {0x280146}, {0x57009A}, 
{0x57009A}, {0x280146}, {0x000000}, {0x57009A}, {0x000000}, {0x57009A}, {0x280146}, 
/* 309 */
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x5E5E5E}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x000000}, {0x280146}, {0x000000}, {0x280146}, {0x000000}, {0x280146}, {0x000000}, 
{0x280146}, {0x000000}, {0x280146}, {0x000000}, {0x280146}, {0x000000}, {0x000000}, 
/* 310 */
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x5E5E5E}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x000000}, {0x57009A}, {0x280146}, {0x140123}, {0x57009A}, {0x280146}, {0x000000}, 
{0x000000}, {0x57009A}, {0x280146}, {0x140123}, {0x57009A}, {0x280146}, {0x000000}, 
/* 311 */
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x5E5E5E}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x140123}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x57009A}, 
{0x280146}, {0x57009A}, {0x280146}, {0x57009A}, {0x280146}, {0x57009A}, {0x280146}, 
{0x000000}, {0x280146}, {0x57009A}, {0x280146}, {0x57009A}, {0x280146}, {0x000000}, 
/* 312 */
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x5E5E5E}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x57009A}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x57009A}, 
{0x57009A}, {0x57009A}, {0x280146}, {0x57009A}, {0x280146}, {0x57009A}, {0x57009A}, 
{0x000000}, {0x57009A}, {0x280146}, {0x57009A}, {0x280146}, {0x57009A}, {0x000000}, 
/* 313 */
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x5E5E5E}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x000000}, {0x517024}, {0x280146}, {0x000000}, {0x517024}, {0x280146}, {0x517024}, 
{0x000000}, {0x000000}, {0x280146}, {0x517024}, {0x000000}, {0x280146}, {0x000000}, 
/* 314 */
{0x000000}, {0x5E5E5E}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x000000}, {0x000000}, {0x000000}, {0x57009A}, {0x57009A}, {0x000000}, {0x000000}, 
{0x000000}, {0x57009A}, {0x000000}, {0x57009A}, {0x000000}, {0x000000}, {0x57009A}, 
{0x000000}, {0x000000}, {0x57009A}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, 
/* 315 */
{0x000000}, {0x5E5E5E}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x000000}, {0x000000}, {0x000000}, {0x57009A}, {0x57009A}, {0x000000}, {0x57009A}, 
{0x000000}, {0x57009A}, {0x000000}, {0x57009A}, {0x000000}, {0x57009A}, {0x000000}, 
{0x000000}, {0x000000}, {0x57009A}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, 
/* 316 */
{0x000000}, {0x5E5E5E}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x000000}, {0x000000}, {0x000000}, {0x57009A}, {0x57009A}, {0x000000}, {0x57009A}, 
{0x000000}, {0x57009A}, {0x000000}, {0x57009A}, {0x000000}, {0x57009A}, {0x000000}, 
{0x57009A}, {0x000000}, {0x57009A}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, 
/* 317 */
{0x000000}, {0x5E5E5E}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x57009A}, {0x000000}, {0x000000}, {0x57009A}, {0x000000}, {0x57009A}, {0x000000}, 
{0x000000}, {0x000000}, {0x57009A}, {0x000000}, {0x57009A}, {0x000000}, {0x57009A}, 
{0x000000}, {0x57009A}, {0x000000}, {0x57009A}, {0x000000}, {0x000000}, {0x000000}, 
/* 318 */
{0x000000}, {0x5E5E5E}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x120020}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x120020}, 
{0x57009A}, {0x000000}, {0x000000}, {0x57009A}, {0x000000}, {0x57009A}, {0x280146}, 
{0x280146}, {0x000000}, {0x57009A}, {0x000000}, {0x57009A}, {0x000000}, {0x57009A}, 
{0x120020}, {0x57009A}, {0x000000}, {0x57009A}, {0x000000}, {0x000000}, {0x120020}, 
/* 350 */
{0x000000}, {0x000000}, {0x587036}, {0x6EB905}, {0x5D8721}, {0x000000}, {0x000000}, 
{0x000000}, {0x151515}, {0x5A565F}, {0x70B015}, {0x71BE03}, {0x517024}, {0x000000}, 
{0x373737}, {0x828282}, {0x868686}, {0x5E5E5E}, {0x70B015}, {0x70B015}, {0x517024}, 
{0x5E5E5E}, {0x868686}, {0x868686}, {0x868686}, {0x5E5E5E}, {0x868686}, {0x5E5E5E}, 
{0x5E5E5E}, {0x868686}, {0x868686}, {0x868686}, {0x868686}, {0x868686}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x000000}, {0x000000}, {0x57009A}, {0x000000}, {0x57009A}, {0x000000}, {0x000000}, 
/* 351 */
{0x000000}, {0x000000}, {0x587036}, {0x6EB905}, {0x5D8721}, {0x000000}, {0x000000}, 
{0x000000}, {0x151515}, {0x5A565F}, {0x70B015}, {0x71BE03}, {0x517024}, {0x000000}, 
{0x373737}, {0x828282}, {0x868686}, {0x5E5E5E}, {0x70B015}, {0x70B015}, {0x517024}, 
{0x5E5E5E}, {0x868686}, {0x868686}, {0x868686}, {0x5E5E5E}, {0x868686}, {0x5E5E5E}, 
{0x5E5E5E}, {0x868686}, {0x868686}, {0x868686}, {0x868686}, {0x868686}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x000000}, {0x57009A}, {0x000000}, {0x57009A}, {0x000000}, {0x57009A}, {0x000000}, 
/* 399 */
{0x000000}, {0x000000}, {0x3D3D3D}, {0x3D3D3D}, {0x3D3D3D}, {0x000000}, {0x000000}, 
{0x000000}, {0x3D3D3D}, {0x868686}, {0x868686}, {0x868686}, {0x3D3D3D}, {0x000000}, 
{0x3D3D3D}, {0x868686}, {0x868686}, {0x868686}, {0x868686}, {0x868686}, {0x3D3D3D}, 
{0x000000}, {0x3D3D3D}, {0x868686}, {0x868686}, {0x868686}, {0x868686}, {0x3D3D3D}, 
{0x000000}, {0x000000}, {0x3D3D3D}, {0x3D3D3D}, {0x3D3D3D}, {0x3D3D3D}, {0x000000}, 
{0x000000}, {0x000000}, {0x280146}, {0x000000}, {0x280146}, {0x000000}, {0x000000}, 
{0x000000}, {0x57009A}, {0x000000}, {0x000000}, {0x57009A}, {0x000000}, {0x000000}, 
/* 400 */
{0x000000}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x000000}, {0x959595}, {0x000000}, {0x959595}, {0x000000}, {0x000000}, 
{0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, 
/* 401 */
{0x000000}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x959595}, {0x000000}, {0x000000}, {0x000000}, {0x959595}, {0x000000}, 
{0x000000}, {0x000000}, {0x000000}, {0x959595}, {0x000000}, {0x000000}, {0x000000}, 
/* 402 */
{0x000000}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x959595}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x959595}, 
{0x000000}, {0x464646}, {0x000000}, {0x000000}, {0x000000}, {0x464646}, {0x000000}, 
{0x000000}, {0x000000}, {0x000000}, {0x959595}, {0x000000}, {0x000000}, {0x000000}, 
/* 403 */
{0x000000}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x959595}, {0x000000}, {0x000000}, {0x767676}, {0x000000}, {0x000000}, {0x959595}, 
{0x000000}, {0x464646}, {0x000000}, {0x000000}, {0x000000}, {0x464646}, {0x000000}, 
{0x000000}, {0x000000}, {0x000000}, {0x959595}, {0x000000}, {0x000000}, {0x000000}, 
/* 404 */
{0x000000}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x000000}, {0x57009A}, {0x000000}, {0x959595}, {0x000000}, {0x000000}, 
{0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, 
/* 405 */
{0x000000}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x000000}, 
{0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, 
{0x5E5E5E}, {0x5E5E5E}, {0x848484}, {0x848484}, {0x848484}, {0x5E5E5E}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x57009A}, {0x000000}, {0x000000}, 
{0x000000}, {0x57009A}, {0x000000}, {0x57009A}, {0x000000}, {0x959595}, {0x000000}, 
{0x000000}, {0x000000}, {0x57009A}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, 
/* 406 */
{0x66A310}, {0x80D10D}, {0x82CC19}, {0x80D10D}, {0x5F8923}, {0x000000}, {0x000000}, 
{0x59782C}, {0x6DBA00}, {0x7ECF0B}, {0x94C057}, {0x7F7A86}, {0x878787}, {0x000000}, 
{0x8C8794}, {0xB2ABBC}, {0x6DBA00}, {0xB2ABBC}, {0xB6B6B6}, {0xB6B6B6}, {0x969696}, 
{0xBCBCBC}, {0xB1B1B1}, {0xB5AFBD}, {0xB1B1B1}, {0xB1B1B1}, {0xB1B1B1}, {0xBCBCBC}, 
{0x000000}, {0x696969}, {0x696969}, {0x696969}, {0x696969}, {0x696969}, {0x000000}, 
{0x000000}, {0x000000}, {0x280146}, {0x000000}, {0x000000}, {0x5E5E5E}, {0x000000}, 
{0x000000}, {0x57009A}, {0x000000}, {0x000000}, {0x959595}, {0x000000}, {0x000000}, 
/* 407 */
{0x66A310}, {0x80D10D}, {0x82CC19}, {0x80D10D}, {0x5F8923}, {0x000000}, {0x000000}, 
{0x59782C}, {0x6DBA00}, {0x7ECF0B}, {0x94C057}, {0x7F7A86}, {0x878787}, {0x000000}, 
{0x8C8794}, {0xB2ABBC}, {0x6DBA00}, {0xB2ABBC}, {0xB6B6B6}, {0xB6B6B6}, {0x969696}, 
{0xBCBCBC}, {0xB1B1B1}, {0xB5AFBD}, {0xB1B1B1}, {0xB1B1B1}, {0xB1B1B1}, {0xBCBCBC}, 
{0x000000}, {0x696969}, {0x696969}, {0x696969}, {0x696969}, {0x696969}, {0x000000}, 
{0x000000}, {0x000000}, {0x2F2F2F}, {0x000000}, {0x2F2F2F}, {0x000000}, {0x000000}, 
{0x000000}, {0x000000}, {0x848484}, {0x000000}, {0x959595}, {0x000000}, {0x000000}, 
/* 408 */
{0x66A310}, {0x80D10D}, {0x82CC19}, {0x80D10D}, {0x5F8923}, {0x000000}, {0x000000}, 
{0x59782C}, {0x6DBA00}, {0x7ECF0B}, {0x94C057}, {0x7F7A86}, {0x878787}, {0x000000}, 
{0x8C8794}, {0xB2ABBC}, {0x6DBA00}, {0xB2ABBC}, {0xB6B6B6}, {0xB6B6B6}, {0x969696}, 
{0xBCBCBC}, {0xB1B1B1}, {0xB5AFBD}, {0xB1B1B1}, {0xB1B1B1}, {0xB1B1B1}, {0xBCBCBC}, 
{0x000000}, {0x000000}, {0x000000}, {0x848484}, {0x1F1F1F}, {0x000000}, {0x000000}, 
{0x000000}, {0x848484}, {0x000000}, {0x5E5E5E}, {0x000000}, {0x848484}, {0x000000}, 
{0x000000}, {0x000000}, {0x848484}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, 
/* 409 */
{0x274200}, {0x80D10D}, {0x82CC19}, {0x80D10D}, {0x274200}, {0x000000}, {0x000000}, 
{0x59782C}, {0x6DBA00}, {0x7ECF0B}, {0x94C057}, {0x7F7A86}, {0x878787}, {0x000000}, 
{0x8C8794}, {0xB2ABBC}, {0x6DBA00}, {0xB2ABBC}, {0xB6B6B6}, {0xB6B6B6}, {0x969696}, 
{0x000000}, {0xB1B1B1}, {0xB5AFBD}, {0xB1B1B1}, {0xB1B1B1}, {0xB1B1B1}, {0x000000}, 
{0x000000}, {0x000000}, {0x000000}, {0x848484}, {0x000000}, {0x000000}, {0x848484}, 
{0x000000}, {0x848484}, {0x000000}, {0x5E5E5E}, {0x000000}, {0x000000}, {0x000000}, 
{0x000000}, {0x000000}, {0x848484}, {0x000000}, {0x000000}, {0x848484}, {0x000000}, 
/* 410 */
{0x66A310}, {0x80D10D}, {0x82CC19}, {0x80D10D}, {0x5F8923}, {0x000000}, {0x000000}, 
{0x59782C}, {0x6DBA00}, {0x7ECF0B}, {0x94C057}, {0x7F7A86}, {0x878787}, {0x000000}, 
{0x8C8794}, {0xB2ABBC}, {0x6DBA00}, {0xB2ABBC}, {0xB6B6B6}, {0xB6B6B6}, {0x969696}, 
{0xBCBCBC}, {0xB1B1B1}, {0xB5AFBD}, {0xB1B1B1}, {0xB1B1B1}, {0xB1B1B1}, {0xBCBCBC}, 
{0x1F1F1F}, {0x000000}, {0x000000}, {0x848484}, {0x1F1F1F}, {0x000000}, {0x959595}, 
{0x5E5E5E}, {0x5E5E5E}, {0x000000}, {0x959595}, {0x000000}, {0x848484}, {0x000000}, 
{0x959595}, {0x000000}, {0x848484}, {0x000000}, {0x000000}, {0x000000}, {0x959595}, 
/* 456 */
{0x000000}, {0x000000}, {0x587036}, {0x6EB905}, {0x5D8721}, {0x000000}, {0x000000}, 
{0x000000}, {0x151515}, {0x5A565F}, {0x70B015}, {0x71BE03}, {0x517024}, {0x000000}, 
{0x373737}, {0x828282}, {0x868686}, {0x5E5E5E}, {0x70B015}, {0x70B015}, {0x517024}, 
{0x5E5E5E}, {0x868686}, {0x868686}, {0x868686}, {0x5E5E5E}, {0x868686}, {0x5E5E5E}, 
{0x5E5E5E}, {0x868686}, {0x868686}, {0x868686}, {0x868686}, {0x868686}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x000000}, {0x000000}, {0x57009A}, {0x000000}, {0x959595}, {0x000000}, {0x000000}, 
/* 457 */
{0x000000}, {0x000000}, {0x587036}, {0x6EB905}, {0x5D8721}, {0x000000}, {0x000000}, 
{0x000000}, {0x151515}, {0x5A565F}, {0x70B015}, {0x71BE03}, {0x517024}, {0x000000}, 
{0x373737}, {0x828282}, {0x868686}, {0x5E5E5E}, {0x70B015}, {0x70B015}, {0x517024}, 
{0x5E5E5E}, {0x868686}, {0x868686}, {0x868686}, {0x5E5E5E}, {0x868686}, {0x5E5E5E}, 
{0x5E5E5E}, {0x868686}, {0x868686}, {0x868686}, {0x868686}, {0x868686}, {0x5E5E5E}, 
{0x000000}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x5E5E5E}, {0x000000}, 
{0x000000}, {0x000000}, {0x959595}, {0x000000}, {0x959595}, {0x000000}, {0x000000}, 
};
