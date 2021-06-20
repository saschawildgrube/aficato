<?php


	function ShellEscapeArgumentString($strArgument)
	{
		$strResult = "'";
		
		if (!IsStringUTF8($strArgument))
		{
			$strArgument = StringEncodeUTF8($strArgument);
		}
		$nLength = mb_strlen($strArgument);	
		
		echo $nLength."\n";
		
		for ($nIndex = 0; $nIndex < $nLength; $nIndex++)
		{
			$nChar = mb_substr($strArgument,$nIndex,1); //$strArgument[$nIndex];
			if ($nChar == "'")
			{
				$strResult .= "'\\".$nChar."'";
			}
			else
			{
				$strResult .= $nChar;
			}
		}
		$strResult .= "'";
		return $strResult;
		
	}

	function ShellExecute(
		$strCommand,
		$arrayParams,
		$arrayEnv,
		$strStdin,
		&$arrayResult)
	{
		$arrayResult = array();
		$arrayResult['command'] = NULL;
		$arrayResult['stdout'] = NULL;
		$arrayResult['stderr'] = NULL;
	  $arrayResult['return'] = NULL;
	   
		if (!is_array($arrayParams))
		{
			return false;	
		}
		if (!is_string($strStdin))
		{
			$strStdin = '';
		}

		foreach ($arrayParams as $strParam)
		{
			$strCommand .= ' '.$strParam;	
		}
		
		if (!is_array($arrayEnv))
		{
			return false;	
		}

		$descriptorspec = array(
			0 => array('pipe', 'r'),  // stdin is a pipe that the child will read from
			1 => array('pipe', 'w'),  // stdout is a pipe that the child will write to
			2 => array('pipe', 'w')   // stderr is a pipe the the child will write to
		);
		$cwd = NULL;
		$pipes = array();
				
		$arrayResult['command'] = $strCommand;
		
		$ea = new CErrorAmnesty();
		
		$ea->Start();
		$process = proc_open(
			$strCommand,
			$descriptorspec,
			$pipes,
			$cwd,
			$arrayEnv);
		$ea->End();
			
		if (is_resource($process)) 
		{
			if ($strStdin != '')
			{
				fwrite($pipes[0],$strStdin);
				fflush($pipes[0]);	
			}
			$arrayResult['stdout'] = stream_get_contents($pipes[1]);
			$arrayResult['stderr'] = stream_get_contents($pipes[2]);
			fclose($pipes[0]); 
			fclose($pipes[1]);
			fclose($pipes[2]); 
			$nReturn = proc_close($process);	
			$arrayResult['return'] = $nReturn;
			return true;
		}
		return false;
	}

	function SudoShellExecute(
		$strCommand,
		$arrayParams,
		$arrayEnv,
		$strStdin,
		&$arrayResult)
	{
		if ($strCommand == '')
		{
			return false;	
		}
		if (!is_array($arrayParams))
		{
			return false;	
		}
		$arrayParams = ArrayAppend(
			array(
				'-n',
				$strCommand),
			$arrayParams);
		$strCommand = 'sudo';
		
		$bResult = ShellExecute(
			$strCommand,
			$arrayParams,
			$arrayEnv,
			$strStdin,
			$arrayResult);
			
		if ($bResult == true)
		{
			$nPosSudoErr = FindString(
				ArrayGetValue($arrayResult,'stderr'),
				'sudo: sorry, a password is required to run sudo');
			if ($nPosSudoErr != -1)	
			{
				return false;	
			}
		}
		return $bResult;
	}
	

