#include <pebble.h>
#define KEY_BG 0
  
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

static GBitmap *s_bg_og;
static GBitmap *s_bg_summer;
static GBitmap *s_bg_autumn;
static GBitmap *s_bg_winter;

static GFont s_time_font;
static GFont s_ampm_font;

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
  //Create the GBitmap, specifying the 'Identifier' chosen earlier, prefixed with RESOURCE_ID_. This will manage the image data:
  // If on Chalk platform
  #if defined(PBL_PLATFORM_CHALK)
  s_bg_summer = gbitmap_create_with_resource(RESOURCE_ID_BG_SUMMER_ROUND);
  s_bg_autumn = gbitmap_create_with_resource(RESOURCE_ID_BG_AUTUMN_ROUND);
  s_bg_winter = gbitmap_create_with_resource(RESOURCE_ID_BG_WINTER_ROUND);
  #endif
  // If on Basalt platform
  #if defined(PBL_PLATFORM_BASALT)
  s_bg_summer = gbitmap_create_with_resource(RESOURCE_ID_BG_SUMMER);
  s_bg_autumn = gbitmap_create_with_resource(RESOURCE_ID_BG_AUTUMN);
  s_bg_winter = gbitmap_create_with_resource(RESOURCE_ID_BG_WINTER);
  #endif
  
  #if defined(PBL_PLATFORM_APLITE)
  s_bg_og = gbitmap_create_with_resource(RESOURCE_ID_BG_ORIGINAL);
  s_bg_summer = s_bg_og;
  s_bg_autumn = s_bg_og;
  s_bg_winter = s_bg_og;
  #endif
  char facebg[20];
  persist_read_string(KEY_BG, facebg, sizeof(facebg));
  
  if(strcmp(facebg,"summer") == 0){
    s_bg_bitmap = s_bg_summer;
  }else if(strcmp(facebg,"autumn") == 0){
    s_bg_bitmap = s_bg_autumn;
  }else if(strcmp(facebg,"winter") == 0){
    s_bg_bitmap = s_bg_winter;
  }else{
    s_bg_bitmap = s_bg_summer;
  }
  
  // Create time TextLayer
  s_time_layer_outline = PBL_IF_RECT_ELSE(text_layer_create(GRect(10, 83, 91, 51)), text_layer_create(GRect(28, 88, 92, 51)));
  text_layer_set_background_color(s_time_layer_outline, GColorClear);
  text_layer_set_text_color(s_time_layer_outline, GColorWhite);
  
  s_time_layer = PBL_IF_RECT_ELSE(text_layer_create(GRect(10, 82, 90, 50)),text_layer_create(GRect(28, 87, 91, 50)));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, PBL_IF_BW_ELSE(GColorBlack, GColorMidnightGreen));

  s_time_layer_AM_outline = PBL_IF_RECT_ELSE(text_layer_create(GRect(105, 89, 33, 33)),text_layer_create(GRect(124, 94, 33, 33)));
  text_layer_set_background_color(s_time_layer_AM_outline,GColorClear);
  text_layer_set_text_color(s_time_layer_AM_outline,GColorWhite);
  
  s_time_layer_AM = PBL_IF_RECT_ELSE(text_layer_create(GRect(104, 88, 32, 32)),text_layer_create(GRect(123, 93, 32, 32)));
  text_layer_set_background_color(s_time_layer_AM,GColorClear);
  text_layer_set_text_color(s_time_layer_AM,PBL_IF_BW_ELSE(GColorBlack, GColorMidnightGreen));
  
  s_time_layer_month = PBL_IF_RECT_ELSE(text_layer_create(GRect(6,51,45,30)),text_layer_create(GRect(24,56,45,30)));
  text_layer_set_background_color(s_time_layer_month,GColorClear);
  text_layer_set_text_color(s_time_layer_month,PBL_IF_BW_ELSE(GColorBlack,GColorBulgarianRose));
  
  s_time_layer_date = PBL_IF_RECT_ELSE(text_layer_create(GRect(45,57,45,30)),text_layer_create(GRect(64,61,45,30)));
  text_layer_set_background_color(s_time_layer_date,GColorClear);
  text_layer_set_text_color(s_time_layer_date,PBL_IF_BW_ELSE(GColorBlack,GColorBulgarianRose));
  
  s_time_layer_day = PBL_IF_RECT_ELSE(text_layer_create(GRect(94, 58, 32, 32)),text_layer_create(GRect(112, 62, 32, 32)));
  text_layer_set_background_color(s_time_layer_day, GColorClear);
  text_layer_set_text_color(s_time_layer_day,PBL_IF_BW_ELSE(GColorBlack,GColorBulgarianRose));
  
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
  
  //Declare a bitmap layer
  //static BitmapLayer *s_bitmap_layer;
  
  //Create bitmap layer and set it to show the GBitmap
  GRect bounds = layer_get_bounds(window_get_root_layer(window));
  s_bitmap_layer = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(s_bitmap_layer, PBL_IF_BW_ELSE(s_bg_og, s_bg_bitmap));
  
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
static void in_recv_handler(DictionaryIterator *iterator, void *context){
  //Get Tuple
  Tuple *t = dict_read_first(iterator);
  if(t)
  {
    switch(t->key)
    {
    case KEY_BG:
      //strcmp compares binary value of two strings, 0 means the two strings are equal
      if(strcmp(t->value->cstring, "summer") == 0){
        bitmap_layer_set_bitmap(s_bitmap_layer, s_bg_summer);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Summer selected");
        persist_write_string(KEY_BG, "summer");
      }
      else if(strcmp(t->value->cstring, "autumn") == 0){
        bitmap_layer_set_bitmap(s_bitmap_layer, s_bg_autumn);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Autumn selected");
        persist_write_string(KEY_BG, "autumn");
      }
      else if(strcmp(t->value->cstring, "winter") == 0){
        bitmap_layer_set_bitmap(s_bitmap_layer, s_bg_winter);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Winter selected");
        persist_write_string(KEY_BG, "winter");
      }
      else{
        APP_LOG(APP_LOG_LEVEL_DEBUG, "God damn it, what happened this time?");
      }
    }
  }
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
  
  app_message_register_inbox_received((AppMessageInboxReceived) in_recv_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  window_stack_push(s_main_window, true);
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