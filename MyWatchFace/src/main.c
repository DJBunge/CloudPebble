#include <pebble.h>
  
static Window *s_main_window;
static Layer *s_time_border;
static TextLayer *s_time_layer;
static Layer *s_connected_border;
static TextLayer *s_connected_layer;
static Layer *s_charged_border;
static TextLayer *s_charged_layer;
static Layer *s_date_border;
static TextLayer *s_date_layer;
static GFont s_time_font;
static GFont connectedFnt;
static Layer *mainLayer;
static int borderSize = 6;
static int borderRadius = 12;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00 AM";
  static char dte[] = "Dec 31 2014";
  
  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) { strftime(buffer, sizeof("00:00"), "%H:%M", tick_time); } else { strftime(buffer, sizeof("00:00 AM"), "%I:%M %p", tick_time); }

  strftime(dte, sizeof("Dec 31 2014"), "%b %d %Y", tick_time);
  
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer); text_layer_set_text(s_date_layer, dte);
}

static void drawBorder(Layer *currentLayer, GContext *currentCtx){
  graphics_context_set_fill_color(currentCtx, GColorWhite);
  graphics_context_set_stroke_color(currentCtx, GColorBlack);
  GRect outerBorder = layer_get_bounds(currentLayer);
  graphics_fill_rect(currentCtx, outerBorder, 0, GCornerNone);
  graphics_context_set_stroke_color(currentCtx, GColorBlack);
  graphics_context_set_fill_color(currentCtx, GColorBlack);
  graphics_fill_rect(currentCtx, outerBorder, borderRadius, GCornersAll);
 
  outerBorder.origin.x= borderSize;
  outerBorder.origin.y= borderSize;
  outerBorder.size.w= outerBorder.size.w-(borderSize*2);
  outerBorder.size.h= outerBorder.size.h-(borderSize*2);
  
  graphics_context_set_stroke_color(currentCtx, GColorBlack);
  graphics_context_set_fill_color(currentCtx, GColorWhite);
  graphics_fill_rect(currentCtx, outerBorder, 0, GCornersAll);
}

static void main_window_load(Window *mainWindow) {
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DigiClock_28));
  connectedFnt = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_Courier_New_11));
  GRect display = layer_get_bounds(window_get_root_layer(mainWindow));
  mainLayer = layer_create(display);
  layer_add_child(window_get_root_layer(mainWindow), mainLayer);
  
  // #region TimeBorder
  //Create layer for time border
  GRect timeBorder = GRect(4, 2, 136, 40);
  s_time_border = layer_create(timeBorder);
  layer_set_update_proc(s_time_border, drawBorder);
  //Add Text Layer for time
  GRect timeDisplay = layer_get_bounds(s_time_border);
  timeDisplay.origin.x=timeDisplay.origin.x+11;
  timeDisplay.size.w= timeDisplay.size.w-22;
  
  s_time_layer = text_layer_create(timeDisplay);
    // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(s_time_layer, GTextOverflowModeWordWrap);
  layer_add_child(s_time_border, text_layer_get_layer(s_time_layer));
  layer_add_child(mainLayer, s_time_border);
  // #endregion
    
  // #region ConnectedText
  //Create layer for connected text border
  GRect connectedBorder = GRect(4, 43, 136, 26);
  s_connected_border = layer_create(connectedBorder);

  GRect connectedDisplay = layer_get_bounds(s_connected_border);
  connectedDisplay.size.w= connectedDisplay.size.w-22;
  connectedDisplay.origin.x=connectedDisplay.origin.x+11;
  connectedDisplay.origin.y=connectedDisplay.origin.y+5;
  s_connected_layer = text_layer_create(connectedDisplay);
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_connected_layer, GColorClear);
  text_layer_set_font(s_connected_layer, connectedFnt);
  text_layer_set_text_alignment(s_connected_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(s_connected_layer, GTextOverflowModeWordWrap);

  layer_add_child(s_connected_border, text_layer_get_layer(s_connected_layer));
  layer_add_child(mainLayer, s_connected_border);
  // #endregion

  // #region ChargedText
  //Create layer for charged text border
  GRect chargedBorder = GRect(4, 70, 136, 26);
  s_charged_border = layer_create(chargedBorder);

  GRect chargedDisplay = layer_get_bounds(s_charged_border);
  chargedDisplay.size.w= chargedDisplay.size.w-22;
  chargedDisplay.origin.x=chargedDisplay.origin.x+11;
  chargedDisplay.origin.y=chargedDisplay.origin.y+5;
  s_charged_layer = text_layer_create(chargedDisplay);
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_charged_layer, GColorClear);
  text_layer_set_font(s_charged_layer, connectedFnt);
  text_layer_set_text_alignment(s_charged_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(s_charged_layer, GTextOverflowModeWordWrap);

  layer_add_child(s_charged_border, text_layer_get_layer(s_charged_layer));
  layer_add_child(mainLayer, s_charged_border);
  // #endregion
  
  // #region Date
  //Create layer for date border
  GRect dateBorder = GRect(4, 97, 136, 65);
  s_date_border = layer_create(dateBorder);
  layer_set_update_proc(s_date_border, drawBorder);

  GRect dateDisplay = layer_get_bounds(s_date_border);
  dateDisplay.origin.x=dateDisplay.origin.x+11;
  dateDisplay.size.w= dateDisplay.size.w-22;
  s_date_layer = text_layer_create(dateDisplay);
  
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_font(s_date_layer, s_time_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(s_date_layer, GTextOverflowModeWordWrap);

  layer_add_child(s_date_border, text_layer_get_layer(s_date_layer));
  layer_add_child(mainLayer, s_date_border);
  // #endregion
}

static void main_window_unload(Window *window) {
    layer_destroy(mainLayer);
    text_layer_destroy(s_time_layer);
    text_layer_destroy(s_date_layer);
    fonts_unload_custom_font(s_time_font);
    fonts_unload_custom_font(connectedFnt);
}
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();
}
static void battery_handler(BatteryChargeState charge_state){
   static char s_battery_buffer[32];
  if (charge_state.is_charging) { snprintf(s_battery_buffer, sizeof(s_battery_buffer), "charging [%d%%]", charge_state.charge_percent); }
  else { snprintf(s_battery_buffer, sizeof(s_battery_buffer), "%d%% charged", charge_state.charge_percent); }
  text_layer_set_text(s_charged_layer, s_battery_buffer);
}

static void connected_handler(bool isConnected){
  if(isConnected){    text_layer_set_text(s_connected_layer, "Connected");  }
  else{    text_layer_set_text(s_connected_layer, "Not Connected");  }
}

static void init() {
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  battery_state_service_subscribe(battery_handler);
  bluetooth_connection_service_subscribe(connected_handler);
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load, .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  connected_handler(bluetooth_connection_service_peek());
  battery_handler(battery_state_service_peek());
  update_time();
}

static void deinit() {
  window_destroy(s_main_window); tick_timer_service_unsubscribe(); bluetooth_connection_service_unsubscribe();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}