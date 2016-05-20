#include <pebble.h>

static Window *window;
static TextLayer *text_layer;
static TextLayer *last_nr_layer;
static TextLayer *points_layer;
static GFont font14, font24;

static int last;
static int points;

static int get_next()
{
  return rand()%10+1;  
}

static void show_points()
{
  static char txt[20];
  snprintf(txt,20,"Points %d", points);  
  
  text_layer_set_text(points_layer, txt);
}

static void show_last()
{
  static char txt[20];
  snprintf(txt,20,"Last %d", last);
    
  text_layer_set_text(last_nr_layer, txt);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  int new_nr=get_next();
  if(new_nr>=last)
  {
    points++;
    text_layer_set_text(text_layer, "Correct");
    show_points();
  }
  else{
    text_layer_set_text(text_layer, "Sorry");
    show_points();
    points=0;
  }
  last=new_nr;
  show_last();
  
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  int new_nr=get_next();
  if(new_nr<=last)
  {
    points++;
    text_layer_set_text(text_layer, "Correct");
    show_points();
  }
  else{
    text_layer_set_text(text_layer, "Sorry");
    show_points();
    points=0;
  }
  last=new_nr;
  show_last();
  
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  font14=fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_EAGLE_14));
  font24=fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_EAGLE_24));
  
  srand(time(0));
  last=rand()%10+1;
  points=0;
  static char txt[20];
  snprintf(txt,20,"Last %d", last);
  
  text_layer = text_layer_create(GRect(0, 72, bounds.size.w, 20));
  text_layer_set_text(text_layer, "Higher or lower?");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, font14);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  
  last_nr_layer=text_layer_create(GRect(0,10,bounds.size.w,30));
  text_layer_set_text(last_nr_layer, txt);
  text_layer_set_text_alignment(last_nr_layer, GTextAlignmentCenter);
  text_layer_set_font(last_nr_layer, font24);
  layer_add_child(window_layer, text_layer_get_layer(last_nr_layer));
  
  
  points_layer=text_layer_create(GRect(0,100,bounds.size.w,20));
  text_layer_set_text(points_layer, "0");
  text_layer_set_text_alignment(points_layer, GTextAlignmentCenter);
  text_layer_set_font(points_layer, font14);
  layer_add_child(window_layer, text_layer_get_layer(points_layer));
  
  
  
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
  fonts_unload_custom_font(font14);
  fonts_unload_custom_font(font24);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}