<?php
	
	error_reporting (E_ALL);
	ini_set('display_errors', 1);
	
	require_once("../_source/env.inc");
	
	require_once(GetWDKDir()."wdk_installer.inc");
	require_once(GetSourceDir()."webservices_directory.inc");
	
	class CMyInstaller extends CInstaller
	{
		function OnInstall()
		{
			if ($this->InstallWebservice("system/log") == false)
			{
				return false;	
			}
			if ($this->InstallWebservice("system/data") == false)
			{
				return false;	
			}
			if ($this->InstallWebservice("system/scheduler") == false)
			{
				return false;	
			}
			if ($this->InstallWebservice("system/session") == false)
			{
				return false;	
			}
			if ($this->InstallWebservice("system/entitlement") == false)
			{
				return false;	
			}
			if ($this->InstallWebservice("system/user") == false)
			{
				return false;	
			}
			if ($this->InstallWebservice("system/test") == false)
			{
				return false;	
			}			

			$arrayPrivileges = array();
			$arrayPrivileges[] = "SYSTEMADMIN";
			if ($this->AddUser("admin","changeme",$arrayPrivileges) == false)
			{
				return false;
			}

			$arrayPrivileges = array();
			$arrayPrivileges[] = "USERADMIN";
			if ($this->AddUser("useradmin","changeme",$arrayPrivileges) == false)
			{
				return false;
			}
			
			return true;
		}
		
		function OnRollback()
		{
			return true;
		}
	}
	
	$installer = new CMyInstaller();
		
