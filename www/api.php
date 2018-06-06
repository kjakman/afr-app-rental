<?php
  header('Content-type:application/json;charset=utf-8');
  require_once('afr-lib/include/set_env.inc'); /** load AFR application */
  require_once('api.inc');
                                       
  $timer=[];
  $timer['start'] = microtime(true);
  $timer['set_env'] = microtime(true);
  
  if(!$_GET) json_error("No input");
  $vars = [];
  foreach($_GET as $k => $v) {
    if(in_array($k,['_v1','_v2','_v3','_v4'])) {
      if($v) $vars[] = ltrim($v, '/');
      unset($_GET[$k]);
    }
  }
  $data = $_GET ?: $_POST; /** todo: look at request header: PUT, DELETE... */
  
  $count = count($vars);
  if(!$count) json_error("Invalid input");
  $method = $vars[0];
  $fn_name = "api_{$method}";

  
  /** security */
  /** check auth token */

  $result = json_error_object("Noop");  
  $class = $function = '';
  if($_CLASSES[$method]) {
    $class = $method;
    json_error("Found class {$method}");
  } elseif(function_exists($fn_name)) {
    $function = $fn_name;
    $result = call_user_func($fn_name, $data);
  } else {
    json_error("{$method} not found");
  }
  
  $pretty = isset($_GET['pretty']) ? JSON_PRETTY_PRINT : 0;
  $json = json_encode($result, $pretty);
  
  if(0) {
    $len = strlen($json);
    $timer['end'] = microtime(true);
    $ts = $timer['end'] - $timer['start']; 
    print_log("Result from {$method} = $len in $ts", 'api', LOG_LEVEL_TEST);
  }
  
  echo($pretty ? "<pre>{$json}</pre>" : $json);exit;
