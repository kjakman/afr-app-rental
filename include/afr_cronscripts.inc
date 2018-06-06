<?php


function afr_cronscript($op) {
  global $__SITE, $__LIVE;  
  $messages = $errors = array();
  $now = now();
  print_log("{$now}: {$__SITE->name} cronscript {$op} start", 'crontab', LOG_LEVEL_ALWAYS);
  
  switch($op) {
    case 'cloudinary-upload':
      require_once 'xml_cloudinary.inc';
      //$result = cloudinary_upload(['site_id' => $__SITE->id, 'parent_type' => 'shortstay'], ['limit' => 20]);
      return $result;
      break;

    default:
      $success = 0;
      // $errors[] = "Unsupported {$__SITE->name} operation $op";
      break;
  }
  
  return json_error_object("{$__SITE->name} $op not found");
}

