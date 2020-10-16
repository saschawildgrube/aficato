<?php

	require_once(GetWDKDir()."wdk_hash.inc");

	class CMyContent extends CContentSystemAdmin
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$arrayModuleConfig = array();
			
			// This can only be a temporary solution!
			$arrayModuleConfig["webservices"] = GetWebservicesDirectory();
			
			$strWebservice = "demo/demo";
			$arrayModuleConfig["webservices"][$strWebservice]["links"] = array(
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=demo&format=xml",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=demo&format=csv",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=demo&format=printr",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=bogus");
			
			$strWebservice = "demo/databasedemo";
			$arrayModuleConfig["webservices"][$strWebservice]["links"] = array(
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=add&item_name=Item1&item_data=Data1",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=set&item_name=Item2&item_data=Data2&item_id=",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=delete&item_id=",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=list&blocksize=100&offset=0",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=list&blocksize=100&offset=1",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=list&blocksize=100&offset=0&sort1=item_id&filter1=item_id&filter1_value=200&filter1_operator=>&filter2=item_id&filter2_value=300&filter2_operator=<");

			$strWebservice = "system/server";			
			$arrayModuleConfig["webservices"][$strWebservice]["links"] = array(
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=info",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=status",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=performance");

				
			$strWebservice = "system/log";			
			$arrayModuleConfig["webservices"][$strWebservice]["links"] = array(
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=verify",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=log&event_severity=DEBUG&reporter_id=test&event_id=test&event_details=This%20is%20a%20test.",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=list&blocksize=100&offset=0",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=list&blocksize=100&offset=1");

			$strWebservice = "system/session";
			$arrayModuleConfig["webservices"][$strWebservice]["links"] = array(
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=cleanup",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=create&owner_id=test&duration=360",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=write&owner_id=test&session_id=",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=read&owner_id=test&session_id=",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=destroy&owner_id=test&session_id=",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=list&blocksize=100&offset=0");
				
			$strWebservice = "system/scheduler";
			$arrayParams["trace"] = "1";
			$arrayParams["job-name"] = "Test Job";
			$arrayParams["job_url"] = StringURLEncode($this->GetWebserviceURL("system/log")."?&trace=1&command=log&reporter_id=test&event_id=test&event_details=".StringURLEncode("This is a crontab test."));
			$arrayParams["active"] = "1";
			$arrayParams["scheduler_minute"] = "*";
			$arrayParams["schedule_hour"] = "*";
			$arrayParams["scheduler_dayofmonth"] = "*";
			$arrayParams["scheduler_dayofweek"] = "*";
			$arrayParams["scheduler_month"] = "*";
			$arrayParams["command"] = "add";
			$strLink = MakeURL(
				$this->GetWebserviceURL($strWebservice),
				$arrayParams);
			$arrayModuleConfig["webservices"][$strWebservice]["links"] = array(
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=crontab",
				$this->GetWebserviceURL($strWebservice)."crontab/?", 
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=execute&job_id=1",
				$strLink);
			
			$strWebservice = "system/data";
			$arrayModuleConfig["webservices"][$strWebservice]["links"] = array(
				$this->GetWebserviceURL($strWebservice)."?command=list&blocksize=100&offset=0&trace=1",
				$this->GetWebserviceURL($strWebservice)."?command=set&data_name=test2&data_content=This%20is%20a%20Test&trace=1",
				$this->GetWebserviceURL($strWebservice)."?command=delete&data_name=test2&trace=1",
				$this->GetWebserviceURL($strWebservice)."?command=set&data_name=lang&data_context_type=USER&data_context_id=test&data_content=en&trace=1",
				$this->GetWebserviceURL($strWebservice)."?command=deletecontext&data_context_type=USER&data_context_id=test&trace=1");

			$strWebservice = "system/entitlement";
			$arrayModuleConfig["webservices"][$strWebservice]["links"] = array(
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=get&user_id=1",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=set&privilege_id=TEST&user_id=1",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=set&privilege_id=PREMIUM&user_id=1",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=delete&user_id=1&privilege_id=TEST",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=deleteall&user_id=1",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=list&blocksize=100&offset=0");
			
			$strWebservice = "system/test";
			$arrayModuleConfig["webservices"][$strWebservice]["links"] = array(
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=testall&alertemail=".StringURLEncode(GetAdminEmail()),
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=testlocal&alertemail=".StringURLEncode(GetAdminEmail()),
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=testrun",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=alert&alertemail=".StringURLEncode(GetAdminEmail()),
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=testlist&getstatus=1",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=testlist&getstatus=1&test_filter1=group_id&test_filter1_value=test&test_filter1_operator=".StringURLEncode("!="),
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=test&testpath=wdk/test/passed",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=test&testpath=wdk/test/undefined",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=test&testpath=wdk/test/failed",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=test&testpath=wdk/test/notest",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=gettest&testpath=wdk/test/passed",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=gettest&testpath=wdk/test/undefined",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=gettest&testpath=wdk/test/failed",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=gettest&testpath=wdk/test/notest",
				"http://".GetRootURL()."test/testwebsite/?content=test-element-toolbar");
	
			$strWebservice = "system/user";
			$arrayModuleConfig["webservices"][$strWebservice]["links"] = array(
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=add&user_name=test",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=set&user_id=1",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=set&user_id=1&password_hash=".MakePasswordHash("1","seed"),
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=get&user_id=1",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=delete&user_id=1",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=list&blocksize=100&offset=0",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=list&blocksize=100&offset=1",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=cleanup");
		
		
			$strWebservice = "security/passwordmanager";
			$arrayModuleConfig["webservices"][$strWebservice]["links"] = array(
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=listkeyrings&blocksize=100&offset=0");



			$strWebservice = "cms/article";
			$arrayModuleConfig["webservices"][$strWebservice]["links"] = array(
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=list&blocksize=100&offset=0",
				$this->GetWebserviceURL($strWebservice)."?trace=1&command=get&name=testtest&language=DE");  
		
		
						
			$this->IncludeModule("system/webservicemanager");
			$webservicemanager = new CWebserviceManagerModule($this->GetWebsite(),"",$arrayModuleConfig);
			$arrayItems["MODULE_WEBSERVICEMANAGER"] = $webservicemanager->RenderModule();
		}
	}
	
