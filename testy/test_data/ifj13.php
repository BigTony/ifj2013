<?php
/** 
 * ifj13.php - Modul pro zlepseni kompatibility PHP a IFJ13
 * 
 * Skript pomaha preklenout zakladni rozdily v obou jazycich pro ucely  
 * testovani, avsak zdaleka neodstranuje vsechny odlisnosti! 
 * Pouziti na serveru Merlin: 
 * 		php -d open_basedir="" ifj13.php vas_skript.php
 * 
 * PHP version 5
 * 
 * @author Zbyněk Křivka <krivka@fit.vutbr.cz>
 * @author Ondřej Navrátil <inavra@fit.vutbr.cz>
 * @author Jakub Křoustek <ikroustek@fit.vutbr.cz>
 * 
 * @version 1, 2013-09-23  
 * 
 */

define("ERR_GET_SUBSTRING", 13);

error_reporting(E_ALL);

if(!defined("STDIN"))
	define("STDIN", fopen('php://stdin', 'r'));
	
if(!function_exists("boolval"))
{
	function boolval($val)
	{
		return (bool)$val;
	}
}

function get_string()
{
	$line = fgets(STDIN);
	if($line === false) //EOF
		return "";
	if($line[strlen($line) - 1] == "\n")
		$line = substr($line, 0, strlen($line) - 1);
	if($line[strlen($line) - 1] == "\r")//for windows \r\n
		$line = substr($line, 0, strlen($line) - 1);
	return $line;
}

function find_string($haystack, $needle)
{
	$haystack = strval($haystack);
	$needle = strval($needle);
		
	if ($needle === "") {
		return 0;
	}
	$result = strpos($haystack, $needle);
	if($result === false)
		return -1;
	return $result;
}

function sort_string($str)
{
	$str = strval($str);
	$changed = true;
	while($changed)
	{
		$changed = false;
		for($i = 1; $i < strlen($str); $i++)
		{
			if(ord($str[$i]) < ord($str[$i-1]))//use ord to prevent numerical strings
			{
				$changed = true;
				$x = $str[$i];
				$str[$i] = $str[$i-1];
				$str[$i-1] = $x;
			}
		}
	}
	return $str;
}

function put_string()
{
	foreach(func_get_args() as $arg)
		print strval($arg);
	return func_num_args();
}

function get_substring($string, $start, $end)
{
	$string = strval($string);
	$start = intval($start);
	$end = intval($end);
	if($start < 0
		|| $end < 0
		|| $start > $end
		|| $start >= strlen($string)
		|| $end > strlen($string))
		die(ERR_GET_SUBSTRING);
	return substr($string, $start, $end - $start);	
}

if(isset($argv[1]))
{
	if(!file_exists($argv[1]))
		die("File $argv[1] does not exist!");
	include($argv[1]);
}

?>