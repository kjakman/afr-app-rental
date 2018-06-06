<?php
function yah_controller_home($app, $options = array()) {
  print_log("Running yah_controller_home", 'app', LOG_LEVEL_TEST);
  afr_controller_home($app, $options);
}

function jaa_controller_home($app, $options = array()) {
  print_log("Running jaa_controller_home", 'app', LOG_LEVEL_TEST);
  afr_controller_home($app, $options);
}

function afr_controller_home($app, $options = array()) {
  //print_log("Running afr_controller_home", 'app', LOG_LEVEL_TEST);
  //die("afr ctrl");
  $version = $app->version;
  $__header = $app->header;   
  $__header->scripts[] = "/afr/bower/geocomplete/jquery.geocomplete.min.js";
  $__header->scripts[] = "/afr/bower/simpleWeather/jquery.simpleWeather.js?v={$version}"; 
  /** min.js is broken, see https://github.com/ctrlaltdylan/jquery.simpleWeather/commit/be92402ff36f7fdf0b669530a9e38776be963f4e */
  $__header->styles[] = "/afr/css/afr_weather.css";
  
  $property_type = $ptype = 'shortstay';
  $siteObj = $app->site;
  $pageObj = $app->page;
  
  $scope = $app->page->scope;
  $city_id = $siteObj->default_city ?: 0;    
  $show_cities = $siteObj->show_cities;
  
  $city_ids = $show_cities ? explode(',', $show_cities) : array();
  if(count($city_ids) == 1) $city_id = $show_cities;
  
  //echo("city=$city_id show=$show_cities".dump($city_ids, true));
  if($city_id) { // city page
    $cityObj = get_object('geo_city', $city_id);
    $cities = array($cityObj);
    $scope->city_id = $city_id;
    $scope->city_name = $cityObj->name;
  } else { // global page
    $cities = find_objects('geo_city', array('id' => array(1000,1001,1021,1026)), array('sort' => 'population DESC'));
  }
  
  $scope->city_count = count($cities);  
  
  if($cityObj) {
    $scope->center = json_encode(array($cityObj->location_lat, $cityObj->location_long));
    $radius = $cityObj->population ? min(round($cityObj->population / 10), 10000) : 10000; // bigger radius for big cities, max 10km
    $bounds = $cityObj->location_bounds ?: json_encode(radius2bounds($cityObj, $radius, true)); 
    $scope->bounds = $bounds;
  }
    
  // featured properties
  $rentalSearch = $_SESSION['property_filters'][$ptype] ?: array('active' => 1, 'client_id' => $siteObj->client_id);
  $rental_ids = $rentalSearch['id'] ?: '';  
  if($city_id) $rentalSearch['city_id'] = $city_id;
  //dump($rentalSearch);
    
  
  $searchOptions = array();
  $searchOptions['fields'] = 'id';
  $allRentals = find_objects($property_type, $rentalSearch, $searchOptions);
  $rentalCount = array('all' => count($allRentals));
  
  $searchOptions = array();
  $searchOptions['limit'] = 3;
  // $searchOptions['sort'] = "created DESC"; // newest first
  $searchOptions['sort'] = "rand()"; // random
  $rentalSearch['id'] = array(2923,61598,69772,2921,2924,2925,35596,38317,42946,49651,61596,61597,69776,69779); 
  $featuredRentals = find_objects($property_type, $rentalSearch, $searchOptions); 
  
  $images = array(
    '/user/1284/object/shortstay/2921/media/full/princess-francesca-1.jpg',
    '/user/1284/object/shortstay/2923/media/full/vanessa-1.jpg', 
    '/user/1284/object/shortstay/2924/media/full/martine-1.jpg',          
    '/user/1284/object/shortstay/38317/media/full/prince-ii-1.jpg');         


  $image_count = count($images);
  $i = 0;
  $rentalPriceMin = array();
  foreach($cities as &$cityObj) {
    $city_id = $cityObj->id;
    $cityObj->link = url_safe($cityObj->name);
    if($cityObj->link == 'the-hague') $cityObj->link = 'thehague'; // historical
    if($cityObj->media) {
      $cityObj->image = print_media($cityObj->media); // future
    } else {
      $index = $i % $image_count;
      $cityObj->image = $images[$index]; // default
    }
    $rentalSearch['city_id'] = $city_id;
    $rentalCount[$city_id] = count_objects($property_type, $rentalSearch);
    $min_price = find_object($property_type, $rentalSearch, array('field' => 'min(rate_day) as rate_day'));
    $rentalPriceMin[$city_id] = is_object($min_price) ? $min_price->rate_day : $min_price;
    $i++;
  }

  // find reviews
  $feedbackSearch = array();
  if($city_id) $feedbackSearch['city_id'] = $city_id;
  $feedbackSearch['client_id'] = $siteObj->client_id;
  $feedbackSearch['published'] = 1;
  $feedbackSearch['parent_type'] = 'reservation';
  $feedbackSearch['property_type'] = $property_type;
  $feedbackSearch['length(feedback_comments)'] = array('>', 50);
  $feedbackSearch['created'] = array('>', date("Y-m-d", "-1y")); // less than a year old (for random order)
  if($rental_ids) $feedbackSearch['apt_id'] = $rental_ids; 
  
  $searchOptions = array();
  $searchOptions['limit'] = 3;
  // $searchOptions['sort'] = "created DESC"; // newest first
  $searchOptions['sort'] = "rand()"; // random
  $feedbackObjs = find_objects('feedback', $feedbackSearch, $searchOptions); 
  //dump($feedbackObjs);
  
  
  //dump($rentalCount);
  //dump($rentalPriceMin);
  $scope->cities = $cities;
  $scope->rental_count = $rentalCount;
  $scope->rental_price_min = $rentalPriceMin;
  $scope->featured_rentals = $featuredRentals;
  $scope->reviews = $feedbackObjs;
  
  $featureCount = count($featuredRentals);
  $cityCount = count($cities);
  $allCount = count($allRentals);
  
  //print_log("Done Running afr_controller_home rcount={$allCount} fcount={$featureCount} ccount={$cityCount} ptype={$ptype} city_id={$city_id}", 'app', LOG_LEVEL_TEST);
  
  return;
}

