<?php

	
	function DetectBot($strClientIP,$strUserAgent)
	{
		$strBotsByUserAgentFile = GetWDKDir().'data/bots-by-useragent.csv';
		if (!IsFile($strBotsByUserAgentFile))
		{
			return "";	
		}
		$arrayBots = FileReadCSV_RFC4180($strBotsByUserAgentFile);
		
		foreach ($arrayBots as $arrayBot)
		{
			$strBotUserAgent = ArrayGetValue($arrayBot,'USERAGENT');
			if (FindString($strUserAgent,$strBotUserAgent) != -1)
			{
				$strBotURL = ArrayGetValue($arrayBot,'URL');
				return $strBotURL;
			}
		}

		if ($strUserAgent == '')
		{
			if ($strClientIP == '72.47.244.17')
			{
				return 'https://www.sslshopper.com/';	
			}	
		}
		return "";
	}



	

