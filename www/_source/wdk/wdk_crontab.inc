<?php


	require_once("wdk_parameter.inc");
	require_once("wdk_datetime.inc");

/*
	
Die Zeit- und Datumsfelder sind:

Minute           0-59
Stunde           0-23
Tag-des-Monats   1-31
Monat            1-12 (oder Namen, siehe unten)
Wochentag        0-7 (0 oder 7 ist Sonntag, oder Namen)

Ein Feld kann ein Stern (*) sein, was immer für
"Erster-Letzter" steht.

Zahlenbereiche sind erlaubt. Bereiche sind zwei Zahlen,
getrennt durch einen Bindestrich. Die angegebenen Grenzen sind
inklusive.
Beispielsweise: 8-11 in "Stunde" bewirkt die Ausführung um 8,
9, 10, 11 Uhr.

Listen sind erlaubt. Eine Liste ist eine Menge von Nummern
(oder Bereichen), getrennt durch Kommata.
Beispiele: "1,2,5,9", "0-4,8-2". 

Schrittweiten können in Verbindung mit Bereichen genutzt
werden. Hinter einem Bereich mit "/<Schrittweite>" angegeben,
bestimmt die Schrittweite, ob Werte innerhalb des Bereiches
übersprungen werden.
Beispiel: "0-23/2" kann unter Stunden benutzt werden, um ein
spezielles Kommando alle zwei Stunden auszuführen. Die
Alternative wäre:
"0,2,4,6,8,10,12,14,16,18,20,22". Schrittweiten sind auch nach
Sternen (*) erlaubt, "alle zwei Stunden" lässt sich auch durch
"* / 2" beschreiben.

Namen können für "Monat" und "Wochentag" benutzt
werden. Benutzen Sie die ersten drei Buchstaben des
entsprechenden Tages oder Monats (Groß-/Kleinschreibung wird
nicht beachtet). Bereiche oder Listen sind mit Namen nicht
erlaubt.

*/


	function CrontabParseDefinition($strCrontabDef,$nMin,$nMax)
	{
		if ($strCrontabDef == "")
		{
			return false;
		}
		if ($nMin > $nMax)
		{
			return false;	
		}
		$arrayValues = array();
		
		$arrayAtomicDefs = explode(",",$strCrontabDef);
		if (ArrayCount($arrayAtomicDefs) == 0)
		{
			return false;	
		}
		
		foreach ($arrayAtomicDefs as $strAtomicDef)
		{
			$arrayFrameAndRepetition = explode("/",$strAtomicDef);
			$strFrameDef = $arrayFrameAndRepetition[0];
			if (ArrayCount($arrayFrameAndRepetition) > 1)
			{
				$nRepetition = intval($arrayFrameAndRepetition[1]);
			}
			else
			{
				$nRepetition = 1;	
			}
			
			$arrayFrameDef = explode("-",$strFrameDef);
			$strFrameMin  = $arrayFrameDef[0];
			if (ArrayCount($arrayFrameDef)>1)
			{
				$strFrameMax  = $arrayFrameDef[1];
			}
			else
			{
				$strFrameMax = "";
			}

			if ($strFrameMax == "" && $strFrameMin != "*") // no frame and NOT all -> a single value
			{
				//print("$strFrameMin ");
				if (StringCheckCharSet($strFrameMin,CHARSET_INTEGER) != true)
				{
					return false;	
				}
				$nValue = intval($strFrameMin);
				//print("$nValue ");
				if (($nValue < $nMin) || ($nValue > $nMax))
				{
					return false;	
				}
				array_push($arrayValues,$nValue);
				return $arrayValues;
			}
			
			if ($strFrameMin == "*")
			{
				$nFrameMin = $nMin;
				$nFrameMax = $nMax;
			}
			else
			{
				if (StringCheckCharSet($strFrameMin,CHARSET_INTEGER) != true)
				{
					return false;	
				}
				if (StringCheckCharSet($strFrameMax,CHARSET_INTEGER) != true)
				{
					return false;	
				}

				$nFrameMin = intval($strFrameMin);
				$nFrameMax = intval($strFrameMax);
				if ($nFrameMin > $nFrameMax)
				{
					return false;
				}

				if (($nFrameMin < $nMin) || ($nFrameMin > $nMax) )
				{
					return false;
				}
				if (($nFrameMin < $nMin) || ($nFrameMin > $nMax) )
				{
					return false;
				}
			}
			
			for ($nValue = $nFrameMin; $nValue <= $nFrameMax; $nValue += $nRepetition )
			{
				array_push($arrayValues,$nValue);
			}
		}
		$arrayValues = array_unique($arrayValues);		
		sort($arrayValues);
		return $arrayValues;
	}
	
	function CrontabCalculateNextDateTime(
		$strPivotDatetime,
		$strMinute,
		$strHour,
		$strDayOfMonth,
		$strMonth,
		$strDayOfWeek,
		&$strError)
	{		
		$arrayMinutes = CrontabParseDefinition($strMinute,0,59);
		$arrayHours = CrontabParseDefinition($strHour,0,23);
		$arrayDayOfMonth = CrontabParseDefinition($strDayOfMonth,1,31);
		$arrayMonth = CrontabParseDefinition($strMonth,1,12);
 		$arrayDayOfWeek = CrontabParseDefinition($strDayOfWeek,0,6);

		if ($arrayMinutes == false)
		{
			$strError = "CRONTAB_DEFINITION_MINUTE";
			return false;
		}
		if ($arrayHours == false)
		{
			$strError = "CRONTAB_DEFINITION_HOUR";
			return false;
		}
		if ($arrayDayOfMonth == false)
		{
			$strError = "CRONTAB_DEFINITION_DAYOFMONTH";
			return false;
		}
		if ($arrayMonth == false)
		{
			$strError = "CRONTAB_DEFINITION_MONTH";
			return false;
		}
 		if ($arrayDayOfWeek == false)
		{
			$strError = "CRONTAB_DEFINITION_DAYOFWEEK";
			return false;
		}



		

		// CHECK PIVOT TIME
		IF (!IsValidDateTime($strPivotDatetime))
		{
			$strPivotDatetime = RenderDateTimeNow();
		}
		
		$timePivot = strtotime($strPivotDatetime);
		if ($timePivot == false)
		{
			$strError = "CRONTAB_PIVOTTIME_INVALID";	
			return false;
		}
		
		
		// SETUP
		$arrayTimePivot = getdate($timePivot);
		$nOriginalPivotDayOfYear = $arrayTimePivot["yday"];
		$nOriginalPivotYear = $arrayTimePivot["year"];
		
		
		// IGNORE SECONDS
		$timePivot = GetTimeAddSeconds($timePivot,-$arrayTimePivot["seconds"]);
		$timeNew = $timePivot;
		
		
		// CALCULATE NEXT MINUTE
		$nPivotMinute = intval($arrayTimePivot["minutes"]);
		for ($nIndex=0;$nIndex < ArrayCount($arrayMinutes);$nIndex++)
		{
			if ($arrayMinutes[$nIndex] > $nPivotMinute )	
			{
				//print("".intval($arrayMinutes[$nIndex])." - ".$nPivotMinute."<br/>");
				$timeNew = GetTimeAddMinutes($timeNew, intval($arrayMinutes[$nIndex]) - $nPivotMinute);
				break;
			}
		}
		if ($nIndex == ArrayCount($arrayMinutes))
		{
			$timeNew = GetTimeAddMinutes($timeNew, 60-$nPivotMinute + $arrayMinutes[0]);
		}
		
	
		
		// CALCULATE NEXT HOUR
		$arrayTimePivot = getdate($timeNew);
		$nPivotHour = intval($arrayTimePivot["hours"]);
		for ($nIndex=0;$nIndex < ArrayCount($arrayHours);$nIndex++)
		{
			if ($arrayHours[$nIndex] >= $nPivotHour )	
			{
				$timeNew = GetTimeAddHours($timeNew, intval($arrayHours[$nIndex]) - $nPivotHour);
				break;
			}
		}
		if ($nIndex == ArrayCount($arrayHours))
		{
			$timeNew = GetTimeAddHours($timeNew, 24-$nPivotHour + $arrayHours[0]);
		}
		
		
		// CALCULATE NEXT DAY / MONTH
		$bMatch = false;
		while ($bMatch == false)
		{
			$arrayTimePivot = getdate($timeNew);
			$nPivotDayOfMonth = intval($arrayTimePivot["mday"]);
			$nPivotDayOfWeek = intval($arrayTimePivot["wday"]);
			$nPivotMonth = intval($arrayTimePivot["mon"]);
			
			if ( ArrayValueExists($arrayDayOfMonth,$nPivotDayOfMonth)
				&& ArrayValueExists($arrayDayOfWeek,$nPivotDayOfWeek)
				&& ArrayValueExists($arrayMonth,$nPivotMonth))
			{
				$bMatch = true;
			}
			else
			{
				$timeNew = GetTimeAddDays($timeNew, 1);
			}
			

				
		}
		
		// DAY SWITCH MINUTE AND HOUR CORRECTION
		if (	$nOriginalPivotDayOfYear != $arrayTimePivot["yday"]
			|| $nOriginalPivotYear != $arrayTimePivot["year"])
		{
			$timeNew = GetTimeAddMinutes($timeNew,
				-$arrayTimePivot["minutes"]
				-(60*$arrayTimePivot["hours"])
				+$arrayMinutes[0]
				+(60*$arrayHours[0]));
		}

		return RenderDateTime($timeNew);	
	}



