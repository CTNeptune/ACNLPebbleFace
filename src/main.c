#include <pebble.h>
//Let's just put this here so I can commit and test out the github thing
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_time_layer_outline;
static TextLayer *s_time_layer_AM;
static TextLayer *s_time_layer_AM_outline;
static TextLayer *s_time_layer_month;
static TextLayer *s_time_layer_date;
static TextLayer *s_time_layer_day;

static BitmapLayer *s_bitmap_layer;

int n = 0;

static GBitmap *s_bg_bitmap;

static GFont *s_time_font;
static GFont *s_ampm_font;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";
  // Write the current hour
  strftime(buffer, sizeof(buffer), "%l:%M", tick_time);
  
  static char pbuffer[] = "xx";
  strftime(pbuffer,sizeof(pbuffer),"%p",tick_time);
  
  static char month[] = "00";
  strftime(month,sizeof(month),"%m",tick_time);
  
  static char date[] = "00";
  strftime(date,sizeof(date),"%d",tick_time);
  
  static char day[] = "xxx";
  strftime(day,sizeof("xxx"),"%a",tick_time);
  
  static char truncDay[2];
  int i;
  for(i=0;i<2;i++){
    truncDay[i]=day[i];
  }
  
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(s_time_layer_outline, buffer);
  text_layer_set_text(s_time_layer_AM,pbuffer);
  text_layer_set_text(s_time_layer_AM_outline,pbuffer);
  text_layer_set_text(s_time_layer_month,month);
  text_layer_set_text(s_time_layer_date,date);
  //text_layer_set_text(s_time_layer_day,day);
  text_layer_set_text(s_time_layer_day,truncDay);
}

static void main_window_load(Window *window) {
  // Create time TextLayer
  s_time_layer_outline = text_layer_create(GRect(10, 83, 91, 51));
  text_layer_set_background_color(s_time_layer_outline, GColorClear);
  text_layer_set_text_color(s_time_layer_outline, GColorWhite);
  
  s_time_layer = text_layer_create(GRect(10, 82, 90, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorMidnightGreen);

  s_time_layer_AM_outline = text_layer_create(GRect(105, 89, 33, 33));
  text_layer_set_background_color(s_time_layer_AM_outline,GColorClear);
  text_layer_set_text_color(s_time_layer_AM_outline,GColorWhite);
  
  s_time_layer_AM = text_layer_create(GRect(104, 88, 32, 32));
  text_layer_set_background_color(s_time_layer_AM,GColorClear);
  text_layer_set_text_color(s_time_layer_AM,GColorMidnightGreen);
  
  s_time_layer_month = text_layer_create(GRect(6,51,45,30));
  text_layer_set_background_color(s_time_layer_month,GColorClear);
  text_layer_set_text_color(s_time_layer_month,GColorBulgarianRose);
  
  s_time_layer_date = text_layer_create(GRect(45,57,45,30));
  text_layer_set_background_color(s_time_layer_date,GColorClear);
  text_layer_set_text_color(s_time_layer_date,GColorBulgarianRose);
  
  s_time_layer_day = text_layer_create(GRect(94, 58, 32, 32));
  text_layer_set_background_color(s_time_layer_day, GColorClear);
  text_layer_set_text_color(s_time_layer_day,GColorBulgarianRose);
  
  // Improve the layout to be more like a watchface
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FACE_FONT_32));
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);
  
  text_layer_set_font(s_time_layer_outline, s_time_font);
  text_layer_set_text_alignment(s_time_layer_outline, GTextAlignmentRight);
  
  s_ampm_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FACE_FONT_18));
  text_layer_set_font(s_time_layer_AM,s_ampm_font);
  text_layer_set_text_alignment(s_time_layer_AM,GTextAlignmentLeft);
  
  text_layer_set_font(s_time_layer_AM_outline,s_ampm_font);
  text_layer_set_text_alignment(s_time_layer_AM_outline, GTextAlignmentLeft);
  
  text_layer_set_font(s_time_layer_month,s_ampm_font);
  text_layer_set_text_alignment(s_time_layer_month, GTextAlignmentCenter);
  
  text_layer_set_font(s_time_layer_date,s_ampm_font);
  text_layer_set_text_alignment(s_time_layer_date, GTextAlignmentCenter);
  
  text_layer_set_font(s_time_layer_day,s_ampm_font);
  text_layer_set_text_alignment(s_time_layer_day, GTextAlignmentCenter);
  
  //Create the GBitmap, specifying the 'Identifier' chosen earlier, prefixed with RESOURCE_ID_. This will manage the image data:
  s_bg_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FACE_BG);
  
  //Declare a bitmap layer
  static BitmapLayer *s_bitmap_layer;
  
  //Create bitmap layer and set it to show the GBitmap
  s_bitmap_layer = bitmap_layer_create(GRect(0,0,144,168));
  bitmap_layer_set_bitmap(s_bitmap_layer,s_bg_bitmap);
  
  //Add the bitmaplayer as a child layer to the window:
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bitmap_layer));
  
  // Add text layer as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer_outline));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer_AM_outline));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer_AM));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer_month));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer_date));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer_day));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_time_layer_outline);
  text_layer_destroy(s_time_layer_AM);
  text_layer_destroy(s_time_layer_AM_outline);
  text_layer_destroy(s_time_layer_month);
  text_layer_destroy(s_time_layer_date);
  text_layer_destroy(s_time_layer_day);
  //Destroy bitmap layer
  bitmap_layer_destroy(s_bitmap_layer);
  gbitmap_destroy(s_bg_bitmap);
  fonts_unload_custom_font(s_time_font);
}
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}
static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Make sure the time is displayed from the start
  update_time();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}