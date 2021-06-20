<?php

	define('TIMESTAMP_INFINITE',2147483647);
	define('DATETIME_INFINITE','2038-01-19 03:14:07');
	
	define('FORMAT_TIME','%H:%M:%S');
	define('FORMAT_DATE','%Y-%m-%d'); 
	define('FORMAT_DATETIME',FORMAT_DATE.' '.FORMAT_TIME);


	function GetTimeNow()
	{
		return microtime(true);	
	}
	
	function RenderDateTimeNow($strTimeZone = '')
	{
		return RenderDateTime(GetTimeNow(),$strTimeZone);
	}
	
	function RenderDateNow($strTimeZone = '')
	{
		return RenderDate(GetTimeNow(),$strTimeZone);
	}	
	
	function RenderTimeNow($strTimeZone = '')
	{
		return RenderTime(GetTimeNow(),$strTimeZone);
	}	

	function RenderDateTimeFormat($time,$strFormat,$strTimeZone = '', $strLanguage = '')
	{
		if ($time == 0)
		{
			return '';	
		}
		if (is_string($time))
		{
			$time = strtotime($time);
			if ($time == false)
			{
				return '';
			}	
		}
		else
		{
			$time = intval($time);
		}
		$strTimeZoneServer = '';
		if ($strTimeZone != '')
		{
			$strTimeZoneServer = date_default_timezone_get();
			date_default_timezone_set($strTimeZone);
		}
		if ($strLanguage != '')
		{
			SetTimeLocale($strLanguage);	
		}
		$strOutput = strftime($strFormat,$time);
		if ($strTimeZoneServer != '')
		{
			date_default_timezone_set($strTimeZoneServer);		
		}
		return $strOutput;
		
	}
	function RenderDateTime($time,$strTimeZone = '')
	{
		return RenderDateTimeFormat($time,FORMAT_DATETIME,$strTimeZone);
	}
	function RenderDate($time,$strTimeZone = '')
	{
		return RenderDateTimeFormat($time,FORMAT_DATE,$strTimeZone);
	}
	function RenderTime($time,$strTimeZone = '')
	{
		return RenderDateTimeFormat($time,FORMAT_TIME,$strTimeZone);
	}
	
	function GetTimeAddSeconds($time,$nSeconds)
	{
		if ($time == false)
		{
			return false;	
		}
		return strtotime(RenderDateTime($time)." $nSeconds sec");
	}

	function GetTimeAddMinutes($time,$nMinutes)
	{
		if ($time == false)
		{
			return false;	
		}
		return strtotime(RenderDateTime($time)." $nMinutes minute");
	}

	function GetTimeAddHours($time,$nHours)
	{
		if ($time == false)
		{
			return false;	
		}
		return strtotime(RenderDateTime($time)." $nHours hour");
	}

	function GetTimeAddDays($time,$nDays)
	{
		if ($time == false)
		{
			return false;	
		}
		return strtotime(RenderDateTime($time)." $nDays day");
	}
	
	function GetTimeAddMonths($time,$nMonths)
	{
		if ($time == false)
		{
			return false;	
		}
		$arrayTime = ParseTime($time);
		$nDay = $arrayTime['DAY'];
		$datetime = new DateTime('@'.$time);
		$bNegative = ($nMonths < 0)?(true):(false);
		$nMonthsAbs  = abs($nMonths);
		$interval = new DateInterval('P'.$nMonthsAbs.'M');
		if ($bNegative == true)
		{
			$interval->invert = 1;	
		}
		$datetime->add($interval);
		
		$time = $datetime->getTimestamp();
		$arrayTime = ParseTime($time);
		$nDayNew = $arrayTime['DAY'];
		if ($nDay != $nDayNew)
		{
			$nLagDays = 0;
			if ($nDay == 31 && $nDayNew == 1)
			{
				$nLagDays = 1;	
			}
			else if ($nDayNew < $nDay)
			{
				$nLagDays = $nDayNew;
			}
			if ($nLagDays != 0)
			{
				$interval = new DateInterval('P'.$nLagDays.'D');
				$interval->invert = 1;
				$datetime->add($interval);
				$time = $datetime->getTimestamp();
			}
		} 
		return $time;
	} 

	function GetTimeAddYears($time,$nYears)
	{
		if ($time == false)
		{
			return false;	
		}
		$arrayTime = ParseTime($time);
		$nDay = $arrayTime['DAY'];
		$datetime = new DateTime('@'.$time);
		$bNegative = ($nYears < 0)?(true):(false);
		$nYearsAbs  = abs($nYears);
		$interval = new DateInterval('P'.$nYearsAbs.'Y');
		if ($bNegative == true)
		{
			$interval->invert = 1;	
		}
		$datetime->add($interval);
		
		$time = $datetime->getTimestamp();
		$arrayTime = ParseTime($time);
		$nDayNew = $arrayTime['DAY'];
		if ($nDay != $nDayNew)
		{
			$nLagDays = 0;
			if ($nDay == 31 && $nDayNew == 1)
			{
				$nLagDays = 1;	
			}
			else if ($nDayNew < $nDay)
			{
				$nLagDays = $nDayNew;
			}
			if ($nLagDays != 0)
			{
				$interval = new DateInterval('P'.$nLagDays.'D');
				$interval->invert = 1;
				$datetime->add($interval);
				$time = $datetime->getTimestamp();
			}
		} 
		return $time;		
		//return strtotime(RenderDateTime($time)." $nYears year");
	}


	function GetTimeWeekBegin($time)
	{
		$arrayDateTime = ParseTime($time);  
		$time = GetTimeAddDays($time,-($arrayDateTime['WEEKDAY']-1));
		$arrayDateTime = ParseTime($time);
		$strDateTime = $arrayDateTime['YEAR'].'-'.$arrayDateTime['MONTH'].'-'.$arrayDateTime['DAY'].' 00:00:00';
		return GetTime($strDateTime);
	}


	function GetTimeWeekEnd($time)
	{
		$arrayDateTime = ParseTime($time);
		$time = GetTimeAddDays($time,7 - $arrayDateTime['WEEKDAY']);
		$arrayDateTime = ParseTime($time);
		$strDateTime = $arrayDateTime['YEAR'].'-'.$arrayDateTime['MONTH'].'-'.$arrayDateTime['DAY'].' 23:59:59';
		$time = GetTime($strDateTime);
		return $time;
	}


	function GetTimeMonthBegin($time)
	{
		$arrayDateTime = ParseTime($time);
		$strDateTime = $arrayDateTime['YEAR'].'-'.$arrayDateTime['MONTH'].'-01 00:00:00';
		return GetTime($strDateTime);
	}


	function GetTimeMonthEnd($time)
	{
		$arrayDateTime = ParseTime($time);
		$strDateTime = $arrayDateTime['YEAR'].'-'.$arrayDateTime['MONTH'].'-01 23:59:59';
		$time = GetTime($strDateTime);
		$time = GetTimeAddMonths($time,1);
		$time = GetTimeAddDays($time,-1);   
		return $time;
	}

	function GetTimeQuarterBegin($time)
	{
		$arrayDateTime = ParseTime($time);
		$nMonth = $arrayDateTime['MONTH'];
		$nMonth -= (($nMonth-1)%3);
		$strDateTime = $arrayDateTime['YEAR'].'-'.$nMonth.'-01 00:00:00';
		$time = GetTime($strDateTime);
		return $time;
	}


	function GetTimeQuarterEnd($time)
	{
		$arrayDateTime = ParseTime($time);
		$nMonth = $arrayDateTime['MONTH'];
		$nMonth += 2 - (($nMonth-1)%3);
		$strDateTime = $arrayDateTime['YEAR'].'-'.$nMonth.'-01 23:59:59';
		$time = GetTime($strDateTime);
		$time = GetTimeAddMonths($time,1);
		$time = GetTimeAddDays($time,-1);   
		return $time;
	}
	



	

	
	function GetTime($strDateTime, $strTimeZone='')
	{
		$strDateTime = ConvertToDateTime($strDateTime);
		$strTimeZoneServer = '';
		if ($strTimeZone != '')
		{
			$strTimeZoneServer = date_default_timezone_get();
			date_default_timezone_set($strTimeZone);
		}
		$time = strtotime($strDateTime);
		if ($strTimeZoneServer != '')
		{
			date_default_timezone_set($strTimeZoneServer);		
		}	
		return $time;
	}


	function IsValidDateTime($strDateTime)
	{
		if (StringLength($strDateTime) != 19)
		{
			return false;
		}
		if (RegExpMatch($strDateTime,"/^([0-9]{4})([-]{1})([0-9]{2})-([0-9]{2}) ([0-9]{2}):([0-9]{2}):([0-9]{2})$/") == false)
		{
			return false;
		}
		$strYear = StringSection($strDateTime,0,4);
		$strMonth = StringSection($strDateTime,5,2);
		$strDay = StringSection($strDateTime,8,2);
		if (checkdate($strMonth,$strDay,$strYear) != true)
		{
			return false;	
		}
		$strHour = StringSection($strDateTime,11,2);
		if (intval($strHour) > 23)
		{
			return false;
		}
		$strMinute = StringSection($strDateTime,14,2);
		if (intval($strMinute) > 59)
		{
			return false;
		}
		$strSecond = StringSection($strDateTime,17,2);
		if (intval($strSecond) > 59)
		{
			return false;
		}
		return true;
	}
	
	function IsValidDate($strDate)
	{
		if (StringLength($strDate) != 10)
		{
			return false;
		}
		if (RegExpMatch($strDate,"/^([0-9]{4})([-]{1})([0-9]{2})-([0-9]{2})$/") == false)
		{
			return false;
		}
		return IsValidDateTime($strDate . ' 00:00:00');
	}
	
	function ConvertToDateTime($strNonISODateTime)
	{
		if (IsValidDateTime($strNonISODateTime))
		{
			return $strNonISODateTime;
		}
		
		// Consider this case: d.mm.yy
		if (StringLength($strNonISODateTime) == 7)
		{
			if (RegExpMatch($strNonISODateTime,"/^[0-9]{1}\.[0-9]{2}\.[0-9]{2}$/") == true)
			{
				$strYear = StringSection($strNonISODateTime,5,2);
				$strMonth = StringSection($strNonISODateTime,2,2); 
				$strDay = StringSection($strNonISODateTime,0,1);
				if (intval($strYear) < 70)
				{
					$strYear = '20'.$strYear;
				}
				else
				{
					$strYear = '19'.$strYear;
				}
				return $strYear.'-'.$strMonth.'-0'.$strDay.' 00:00:00';
			}
		}
		
		// Consider this case: dd.mm.yy
		else if (StringLength($strNonISODateTime) == 8)
		{
			if (RegExpMatch($strNonISODateTime,"/^[0-9]{2}\.[0-9]{2}\.[0-9]{2}$/") == true)
			{
				$strYear = StringSection($strNonISODateTime,6,2);
				$strMonth = StringSection($strNonISODateTime,3,2); 
				$strDay = StringSection($strNonISODateTime,0,2);
				if (intval($strYear) < 70)
				{
					$strYear = '20'.$strYear;
				}
				else
				{
					$strYear = '19'.$strYear;
				}
				return $strYear.'-'.$strMonth.'-'.$strDay;
			}
		}			
		
		$time = strtotime($strNonISODateTime);
		return RenderDateTime($time);
	}	

	function ConvertToDate($strNonISODate)
	{
		if (IsValidDate($strNonISODate))
		{
			return $strNonISODate;
		}
		
		// Consider this case: d.mm.yy
		if (StringLength($strNonISODate) == 7)
		{
			if (RegExpMatch($strNonISODate,"/^[0-9]{1}\.[0-9]{2}\.[0-9]{2}$/") == true)
			{
				$strYear = StringSection($strNonISODate,5,2);
				$strMonth = StringSection($strNonISODate,2,2); 
				$strDay = StringSection($strNonISODate,0,1);
				if (intval($strYear) < 70)
				{
					$strYear = '20'.$strYear;
				}
				else
				{
					$strYear = '19'.$strYear;
				}
				return $strYear.'-'.$strMonth.'-0'.$strDay;
			}
		}
		
		// Consider this case: dd.mm.yy
		else if (StringLength($strNonISODate) == 8)
		{
			if (RegExpMatch($strNonISODate,"/^[0-9]{2}\.[0-9]{2}\.[0-9]{2}$/") == true)
			{
				$strYear = StringSection($strNonISODate,6,2);
				$strMonth = StringSection($strNonISODate,3,2); 
				$strDay = StringSection($strNonISODate,0,2);
				if (intval($strYear) < 70)
				{
					$strYear = '20'.$strYear;
				}
				else
				{
					$strYear = '19'.$strYear;
				}
				return $strYear.'-'.$strMonth.'-'.$strDay;
			}
		}				
		
		$time = strtotime($strNonISODate);
		return RenderDate($time);

	}
	
	function ParseDateTime($strDateTime)
	{
		if (!IsValidDateTime($strDateTime))
		{
			return false;
		}
		$arrayResult1 = date_parse_from_format('Y-m-d H:i:s',$strDateTime);  
		if (is_array($arrayResult1) == false)
		{
			return false;	
		}
		if ($arrayResult1['error_count'] > 0)
		{
			return false;	
		}
		$arrayResult2 = strptime($strDateTime,FORMAT_DATETIME);
		if (!is_array($arrayResult2))   
		{
			return false;	
		}

		$arrayResult = array();
		$arrayResult['YEAR'] = $arrayResult1['year'];
		$arrayResult['MONTH'] = $arrayResult1['month'];
		$arrayResult['DAY'] = $arrayResult1['day'];
		$arrayResult['HOUR'] = $arrayResult1['hour'];
		$arrayResult['MINUTE'] = $arrayResult1['minute'];
		$arrayResult['SECOND'] = $arrayResult1['second'];
		$arrayResult['WEEKDAY'] = ($arrayResult2['tm_wday'] > 0)?($arrayResult2['tm_wday']):(7);
		//$arrayResult['DST'] = GetBoolvalue(ArrayGetValue($arrayResult1,'is_dst')); // does not work. php documentation is plain wrong about this
		$arrayResult['DAYOFYEAR'] = $arrayResult2['tm_yday']+1;
		//$arrayResult['MINUTEOFDAY'] = ; // Calculation of minutes on DST border days?
		return $arrayResult;
	}

	function ParseDate($strDate)
	{
		if (!IsValidDate($strDate))
		{
			return false;
		}
		return ParseDateTime($strDate.' 00:00:00');
	}

	function ParseTime($time)
	{
		return ParseDateTime(RenderDateTime($time));
	}


	function GetDateTimeLocalFormat($strCountryID)
	{
		return GetDateLocalFormat($strCountryID) .' '.GetTimeLocalFormat($strCountryID);
	}

	// http://en.wikipedia.org/wiki/Date_and_time_notation_by_country
	function GetDateLocalFormat($strCountryID)
	{
		switch ($strCountryID)
		{
			case 'DEU':
				return '%d.%m.%Y';
			case 'USA':
				return '%m/%d/%Y';		
			case 'GBR':
				return '%d/%m/%Y';
			case 'FRA':
				return '%d/%m/%Y';
			case 'ESP':
				return '%d/%m/%Y';
			default:
				return '%Y-%m-%d';
		}
	}

	function GetTimeLocalFormat($strCountryID)
	{
		switch ($strCountryID)
		{
			case 'GBR':
			case 'USA':
				return '%I:%M:%S%P';
			default:
				return '%H:%M:%S';
		}
	}

	function SetTimeLocale($strLanguage)
	{
		$strLocale = GetLocale('',$strLanguage);
		if ($strLocale == '')
		{
			return false;	
		}
		$bResult = setlocale(LC_TIME,$strLocale);
		if ($bResult == false)
		{
			return false;
		}
		return true;
	}
	
	function GetTimeLocale()
	{
		return setlocale(LC_TIME,0);	
	}
	
	function IsValidTimezoneID($strTimezone)
	{
		return ArrayValueExists(GetAllTimezoneIDs(),$strTimezone);	
	}
	
	function GetAllTimezoneIDs()
	{
		return array(
			'UTC',
			'Africa/Abidjan',
			'Africa/Accra',
			'Africa/Addis_Ababa',
			'Africa/Algiers',
			'Africa/Asmara',
			'Africa/Asmera',
			'Africa/Bamako',
			'Africa/Bangui',
			'Africa/Banjul',
			'Africa/Bissau',
			'Africa/Blantyre',
			'Africa/Brazzaville',
			'Africa/Bujumbura',
			'Africa/Cairo',
			'Africa/Casablanca',
			'Africa/Ceuta',
			'Africa/Conakry',
			'Africa/Dakar',
			'Africa/Dar_es_Salaam',
			'Africa/Djibouti',
			'Africa/Douala',
			'Africa/El_Aaiun',
			'Africa/Freetown',
			'Africa/Gaborone',
			'Africa/Harare',
			'Africa/Johannesburg',
			'Africa/Kampala',
			'Africa/Khartoum',
			'Africa/Kigali',
			'Africa/Kinshasa',
			'Africa/Lagos',
			'Africa/Libreville',
			'Africa/Lome',
			'Africa/Luanda',
			'Africa/Lubumbashi',
			'Africa/Lusaka',
			'Africa/Malabo',
			'Africa/Maputo',
			'Africa/Maseru',
			'Africa/Mbabane',
			'Africa/Mogadishu',
			'Africa/Monrovia',
			'Africa/Nairobi',
			'Africa/Ndjamena',
			'Africa/Niamey',
			'Africa/Nouakchott',
			'Africa/Ouagadougou',
			'Africa/Porto-Novo',
			'Africa/Sao_Tome',
			'Africa/Timbuktu',
			'Africa/Tripoli',
			'Africa/Tunis',
			'Africa/Windhoek',
			'America/Adak',
			'America/Anchorage',
			'America/Anguilla',
			'America/Antigua',
			'America/Araguaina',
			'America/Argentina/Buenos_Aires',
			'America/Argentina/Catamarca',
			'America/Argentina/ComodRivadavia',
			'America/Argentina/Cordoba',
			'America/Argentina/Jujuy',
			'America/Argentina/La_Rioja',
			'America/Argentina/Mendoza',
			'America/Argentina/Rio_Gallegos',
			'America/Argentina/San_Juan',
			'America/Argentina/San_Luis',
			'America/Argentina/Tucuman',
			'America/Argentina/Ushuaia',
			'America/Aruba',
			'America/Asuncion',
			'America/Atikokan',
			'America/Atka',
			'America/Bahia',
			'America/Barbados',
			'America/Belem',
			'America/Belize',
			'America/Blanc-Sablon',
			'America/Boa_Vista',
			'America/Bogota',
			'America/Boise',
			'America/Buenos_Aires',
			'America/Cambridge_Bay',
			'America/Campo_Grande',
			'America/Cancun America/Caracas',
			'America/Catamarca',
			'America/Cayenne',
			'America/Cayman',
			'America/Chicago',
			'America/Chihuahua',
			'America/Coral_Harbour',
			'America/Cordoba',
			'America/Costa_Rica',
			'America/Cuiaba',
			'America/Curacao',
			'America/Danmarkshavn',
			'America/Dawson',
			'America/Dawson_Creek',
			'America/Denver',
			'America/Detroit',
			'America/Dominica',
			'America/Edmonton',
			'America/Eirunepe',
			'America/El_Salvador',
			'America/Ensenada',
			'America/Fort_Wayne',
			'America/Fortaleza',
			'America/Glace_Bay',
			'America/Godthab',
			'America/Goose_Bay',
			'America/Grand_Turk',
			'America/Grenada',
			'America/Guadeloupe',
			'America/Guatemala',
			'America/Guayaquil',
			'America/Guyana',
			'America/Halifax',
			'America/Havana',
			'America/Hermosillo',
			'America/Indiana/Indianapolis',
			'America/Indiana/Knox',
			'America/Indiana/Marengo',
			'America/Indiana/Petersburg',
			'America/Indiana/Tell_City',
			'America/Indiana/Vevay',
			'America/Indiana/Vincennes',
			'America/Indiana/Winamac',
			'America/Indianapolis',
			'America/Inuvik',
			'America/Iqaluit',
			'America/Jamaica',
			'America/Jujuy',
			'America/Juneau',
			'America/Kentucky/Louisville',
			'America/Kentucky/Monticello',
			'America/Knox_IN',
			'America/La_Paz',
			'America/Lima',
			'America/Los_Angeles',
			'America/Louisville',
			'America/Maceio',
			'America/Managua',
			'America/Manaus',
			'America/Marigot',
			'America/Martinique',
			'America/Mazatlan',
			'America/Mendoza',
			'America/Menominee',
			'America/Merida',
			'America/Mexico_City',
			'America/Miquelon',
			'America/Moncton',
			'America/Monterrey',
			'America/Montevideo',
			'America/Montreal',
			'America/Montserrat',
			'America/Nassau',
			'America/New_York',
			'America/Nipigon',
			'America/Nome',
			'America/Noronha',
			'America/North_Dakota/Center',
			'America/North_Dakota/New_Salem',
			'America/Panama',
			'America/Pangnirtung',
			'America/Paramaribo',
			'America/Phoenix',
			'America/Port-au-Prince',
			'America/Port_of_Spain',
			'America/Porto_Acre',
			'America/Porto_Velho',
			'America/Puerto_Rico',
			'America/Rainy_River',
			'America/Rankin_Inlet',
			'America/Recife',
			'America/Regina',
			'America/Resolute',
			'America/Rio_Branco',
			'America/Rosario',
			'America/Santiago',
			'America/Santo_Domingo',
			'America/Sao_Paulo',
			'America/Scoresbysund',
			'America/Shiprock',
			'America/St_Barthelemy',
			'America/St_Johns',
			'America/St_Kitts',
			'America/St_Lucia',
			'America/St_Thomas',
			'America/St_Vincent',
			'America/Swift_Current',
			'America/Tegucigalpa',
			'America/Thule',
			'America/Thunder_Bay',
			'America/Tijuana',
			'America/Toronto',
			'America/Tortola',
			'America/Vancouver',
			'America/Virgin',
			'America/Whitehorse',
			'America/Winnipeg',
			'America/Yakutat',
			'America/Yellowknife',
			'Antarctica/Casey',
			'Antarctica/Davis',
			'Antarctica/DumontDUrville',
			'Antarctica/Mawson',
			'Antarctica/McMurdo',
			'Antarctica/Palmer',
			'Antarctica/Rothera',
			'Antarctica/South_Pole',
			'Antarctica/Syowa',
			'Antarctica/Vostok',
			'Arctic/Longyearbyen',
			'Asia/Aden',
			'Asia/Almaty',
			'Asia/Amman',
			'Asia/Anadyr',
			'Asia/Aqtau',
			'Asia/Aqtobe',
			'Asia/Ashgabat',
			'Asia/Ashkhabad',
			'Asia/Baghdad',
			'Asia/Bahrain',
			'Asia/Baku',
			'Asia/Bangkok',
			'Asia/Beirut',
			'Asia/Bishkek',
			'Asia/Brunei',
			'Asia/Calcutta',
			'Asia/Choibalsan',
			'Asia/Chongqing',
			'Asia/Chungking',
			'Asia/Colombo',
			'Asia/Dacca',
			'Asia/Damascus',
			'Asia/Dhaka',
			'Asia/Dili',
			'Asia/Dubai',
			'Asia/Dushanbe',
			'Asia/Gaza',
			'Asia/Harbin',
			'Asia/Ho_Chi_Minh',
			'Asia/Hong_Kong',
			'Asia/Hovd',
			'Asia/Irkutsk',
			'Asia/Istanbul',
			'Asia/Jakarta',
			'Asia/Jayapura',
			'Asia/Jerusalem',
			'Asia/Kabul',
			'Asia/Kamchatka',
			'Asia/Karachi',
			'Asia/Kashgar',
			'Asia/Katmandu',
			'Asia/Kolkata',
			'Asia/Krasnoyarsk',
			'Asia/Kuala_Lumpur',
			'Asia/Kuching',
			'Asia/Kuwait',
			'Asia/Macao',
			'Asia/Macau',
			'Asia/Magadan',
			'Asia/Makassar',
			'Asia/Manila',
			'Asia/Muscat',
			'Asia/Nicosia',
			'Asia/Novosibirsk',
			'Asia/Omsk',
			'Asia/Oral',
			'Asia/Phnom_Penh',
			'Asia/Pontianak',
			'Asia/Pyongyang',
			'Asia/Qatar',
			'Asia/Qyzylorda',
			'Asia/Rangoon',
			'Asia/Riyadh',
			'Asia/Saigon',
			'Asia/Sakhalin',
			'Asia/Samarkand',
			'Asia/Seoul',
			'Asia/Shanghai',
			'Asia/Singapore',
			'Asia/Taipei',
			'Asia/Tashkent',
			'Asia/Tbilisi',
			'Asia/Tehran',
			'Asia/Tel_Aviv',
			'Asia/Thimbu',
			'Asia/Thimphu',
			'Asia/Tokyo',
			'Asia/Ujung_Pandang',
			'Asia/Ulaanbaatar',
			'Asia/Ulan_Bator',
			'Asia/Urumqi',
			'Asia/Vientiane',
			'Asia/Vladivostok',
			'Asia/Yakutsk',
			'Asia/Yekaterinburg',
			'Asia/Yerevan',
			'Atlantic/Azores',
			'Atlantic/Bermuda',
			'Atlantic/Canary',
			'Atlantic/Cape_Verde',
			'Atlantic/Faeroe',
			'Atlantic/Faroe',
			'Atlantic/Jan_Mayen',
			'Atlantic/Madeira',
			'Atlantic/Reykjavik',
			'Atlantic/South_Georgia',
			'Atlantic/St_Helena',
			'Atlantic/Stanley',
			'Australia/ACT',
			'Australia/Adelaide',
			'Australia/Brisbane',
			'Australia/Broken_Hill',
			'Australia/Canberra',
			'Australia/Currie',
			'Australia/Darwin',
			'Australia/Eucla',
			'Australia/Hobart',
			'Australia/LHI',
			'Australia/Lindeman',
			'Australia/Lord_Howe',
			'Australia/Melbourne',
			'Australia/North',
			'Australia/NSW',
			'Australia/Perth',
			'Australia/Queensland',
			'Australia/South',
			'Australia/Sydney',
			'Australia/Tasmania',
			'Australia/Victoria',
			'Australia/West',
			'Australia/Yancowinna',
			'Europe/Amsterdam',
			'Europe/Andorra',
			'Europe/Athens',
			'Europe/Belfast',
			'Europe/Belgrade',
			'Europe/Berlin',
			'Europe/Bratislava',
			'Europe/Brussels',
			'Europe/Bucharest',
			'Europe/Budapest',
			'Europe/Chisinau',
			'Europe/Copenhagen',
			'Europe/Dublin',
			'Europe/Gibraltar',
			'Europe/Guernsey',
			'Europe/Helsinki',
			'Europe/Isle_of_Man',
			'Europe/Istanbul',
			'Europe/Jersey',
			'Europe/Kaliningrad',
			'Europe/Kiev',
			'Europe/Lisbon',
			'Europe/Ljubljana',
			'Europe/London',
			'Europe/Luxembourg',
			'Europe/Madrid',
			'Europe/Malta',
			'Europe/Mariehamn',
			'Europe/Minsk',
			'Europe/Monaco',
			'Europe/Moscow',
			'Europe/Nicosia',
			'Europe/Oslo',
			'Europe/Paris',
			'Europe/Podgorica',
			'Europe/Prague',
			'Europe/Riga',
			'Europe/Rome',
			'Europe/Samara',
			'Europe/San_Marino',
			'Europe/Sarajevo',
			'Europe/Simferopol',
			'Europe/Skopje',
			'Europe/Sofia',
			'Europe/Stockholm',
			'Europe/Tallinn',
			'Europe/Tirane',
			'Europe/Tiraspol',
			'Europe/Uzhgorod',
			'Europe/Vaduz',
			'Europe/Vatican',
			'Europe/Vienna',
			'Europe/Vilnius',
			'Europe/Volgograd',
			'Europe/Warsaw',
			'Europe/Zagreb',
			'Europe/Zaporozhye',
			'Europe/Zurich',
			'Indian/Antananarivo',
			'Indian/Chagos',
			'Indian/Christmas',
			'Indian/Cocos',
			'Indian/Comoro',
			'Indian/Kerguelen',
			'Indian/Mahe',
			'Indian/Maldives',
			'Indian/Mauritius',
			'Indian/Mayotte',
			'Indian/Reunion',
			'Pacific/Apia',
			'Pacific/Auckland',
			'Pacific/Chatham',
			'Pacific/Easter',
			'Pacific/Efate',
			'Pacific/Enderbury',
			'Pacific/Fakaofo',
			'Pacific/Fiji',
			'Pacific/Funafuti',
			'Pacific/Galapagos',
			'Pacific/Gambier',
			'Pacific/Guadalcanal',
			'Pacific/Guam',
			'Pacific/Honolulu',
			'Pacific/Johnston',
			'Pacific/Kiritimati',
			'Pacific/Kosrae',
			'Pacific/Kwajalein',
			'Pacific/Majuro',
			'Pacific/Marquesas',
			'Pacific/Midway',
			'Pacific/Nauru',
			'Pacific/Niue',
			'Pacific/Norfolk',
			'Pacific/Noumea',
			'Pacific/Pago_Pago',
			'Pacific/Palau',
			'Pacific/Pitcairn',
			'Pacific/Ponape',
			'Pacific/Port_Moresby',
			'Pacific/Rarotonga',
			'Pacific/Saipan',
			'Pacific/Samoa',
			'Pacific/Tahiti',
			'Pacific/Tarawa',
			'Pacific/Tongatapu',
			'Pacific/Truk',
			'Pacific/Wake',
			'Pacific/Wallis',
			'Pacific/Yap');
	}
	

