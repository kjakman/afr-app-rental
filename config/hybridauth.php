<?php

/**
 * HybridAuth
 * http://hybridauth.sourceforge.net | http://github.com/hybridauth/hybridauth
 * (c) 2009-2015, HybridAuth authors | http://hybridauth.sourceforge.net/licenses.html
 */
// ----------------------------------------------------------------------------------------
//	HybridAuth Config file: http://hybridauth.sourceforge.net/userguide/Configuration.html
// ----------------------------------------------------------------------------------------

$protocol = (!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] !== 'off' || $_SERVER['SERVER_PORT'] == 443 || $_SERVER['HTTP_X_FORWARDED_PROTO'] == 'https') ? "https://" : "http://";
$domainName = $_SERVER['HTTP_HOST'].($trailing_slash ? '/' : '');
$live = strpos($domainName, 'www') === 0 ? 1 : 0;
$config = array(
    "base_url" => "{$protocol}{$domainName}/auth/",
    "providers" => array(
        "Google" => array(
            "enabled" => true,
            "keys" => array("id" => "400037276628.apps.googleusercontent.com", "secret" => "NehiLyzBlBaYC0uFmKOEaOem"),
        ),
        "Facebook" => array(
            "enabled" => true,
            "keys" => array("id" => "293591894072016", "secret" => "0198440ad456c7047d23aee0fa04795c"),
            "scope" => "email,user_about_me,user_birthday,user_hometown,user_location,public_profile,user_friends",
            "trustForwarded" => true,
        )
    ),
    "debug_mode" => 0,
    //"debug_file" => "{$log_dir}hybridauth.txt",
);
return $config;
