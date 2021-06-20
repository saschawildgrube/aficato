<?php

	require_once("wdk_shell.inc");

	function IPTablesExecute(
		$arrayParams,
		&$arrayResult)
	{
		return SudoShellExecute(
			"/sbin/iptables",
			$arrayParams,
			array(),
			"",
			$arrayResult);
	}

	function IPTablesIsExecutable()
	{
		$arrayResult = array();
		$bResult = IPTablesExecute(
			array("-h"),
			$arrayResult);
		return $bResult;
	}

	function IPTablesCheckRule($strRule)
	{
		$strParameter = "-C ".$strRule;
		$arrayResult = array();
		$bResult = IPTablesExecute(
			array($strParameter),
			$arrayResult);
		if ($bResult == false)
		{
			return false; // could not execute iptables	
		}
		if (ArrayGetValue($arrayResult,"return") == 1)
		{
			return false;	// no matching rule was found
		}
		return true; // matching rule was found
	}


	function IPTablesAppendRule($strRule)
	{
		$strParameter = "-A ".$strRule;
		$arrayResult = array();
		$bResult = IPTablesExecute(
			array($strParameter),
			$arrayResult);

		return $bResult;	
	}

	function IPTablesDeleteRule($strRule)
	{
		$strParameter = "-D ".$strRule;
		$arrayResult = array();
		$bResult = IPTablesExecute(
			array($strParameter),
			$arrayResult);

		// we ignore further results. The return value should indicate if the rules has been removed not if the rule existed before.

		return $bResult;	
	}

	function IPTablesIsIPBlocked($strIP)
	{
		return IPTablesCheckRule("INPUT -s $strIP -j DROP");	
	}
	
	function IPTablesBlockIP($strIP)
	{
		return IPTablesAppendRule("INPUT -s $strIP -j DROP");	
	}
	
	function IPTablesUnblockIP($strIP)
	{
		return IPTablesDeleteRule("INPUT -s $strIP -j DROP");
	}

