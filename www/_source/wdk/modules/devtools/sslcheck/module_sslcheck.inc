<?php

	require_once(GetWDKDir()."wdk_module.inc");
	require_once(GetWDKDir()."wdk_ssl.inc");
	
	class CSSLCheckModule extends CModule
	{
		
		private $m_arrayErrors;
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"devtools/sslcheck",
				$strInstanceID,
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			$this->m_arrayErrors = array();
			
			$arrayToolbarConfig = array();
			/*
			$arrayToolbarConfig = array(
				"start" => array("help"),
				"help" => array("start"),				
				"action1" => array("help","start"),
				"action1submit" => array("help","start","action1"),
				"action2" => array("help","start"),
				"action2submit" => array("help","start","action2")
				);
			*/
			$this->SetToolbarConfig($arrayToolbarConfig);
			
			return true;
		}
		
	
		function CallbackRenderModule($strState)
		{
			$this->Trace("CallbackRenderModule(\"$strState\")");
			switch ($strState)
			{
				case "check":
					return $this->StateCheck();
				case "":
					$this->SetState("start");
				default:
					return $this->StateStart();
			}
		}
		
	
		function RenderToolbar()
		{
			$toolbarDef = new CToolbarDefinition();



/*
			if ($this->IsToolVisible("recheck"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "check";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "execute", "?TID_HOSTCHECK_TOOL_RECHECK?");
			}
			*/


			$toolbar = new CToolbarElement($this->GetWebsite(),"default");
			$this->SetOutputItem("TOOLBAR",$toolbar->RenderToolbar($toolbarDef)); 
		}		
		
		/*
		function StateHelp()
		{
			$this->RenderToolBar();
			
			return "help";
		}	
		*/	
		
		function StateStart()
		{
			$this->RenderToolBar();
			
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "check";		

			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);

			$strHost = $this->GetModuleVar("host");

			$formDef->AddControlText("HOST",$this->GetModuleVarName("host"),"?TID_SSLCHECK_LABEL_HOST?",$strHost,100,255); 
			$formDef->AddControlButtonSubmit("?TID_SSLCHECK_BUTTON_CHECK?");
			
			$form = new CFormElement($this->GetWebsite(),"default");
			
			$this->SetOutputItem( 
				"FORM",
				$form->RenderForm(
					$formDef,
					array(),					
					$this->m_arrayErrors));
					
			$this->RenderToolBar();
			
			if ($this->GetOutputItem("RESULT") == "")
			{
				$this->SetOutputItem("RESULT","");	
			}

			return "start";
		}
		
		function StateCheck()
		{
			$strHost = $this->GetModuleVar("host");
			$strHost = StringTrim($strHost);
			$this->SetModuleVar("host",$strHost); 

			if (IsValidHostName($strHost) == false)
			{
				$this->m_arrayErrors['HOST'] = '?TID_SSLCHECK_ERROR_HOST_INVALID?';
				return $this->StateStart();	
			}

			$arrayCertificates = GetSSLCertificatesFromHost($strHost);
			if (!is_array($arrayCertificates))
			{
				$this->m_arrayErrors['HOST'] = '?TID_SSLCHECK_ERROR_SSL?';
				return $this->StateStart();	
			}
			
			$arrayCert = $arrayCertificates[0];
			if (!is_array($arrayCert))
			{
				$this->m_arrayErrors['HOST'] = '?TID_SSLCHECK_ERROR_SSL?';
				return $this->StateStart();	
			}			


			$table = new CTableElement($this->GetWebsite(),'default');

			$arrayTable = array();
			
			// Hostname
			$arrayTable[] = array('?TID_SSLCHECK_LABEL_HOSTNAME?',$strHost);
			
			// Get First Certificate in Chain
			
			$arrayCert = $arrayCertificates[0];
			
			
			// SSL Certificate Validity
			$strValidTo = ArrayGetValue($arrayCert,'validTo_time_t');
			if ($strValidTo != '')
			{
				$timeValidTo = intval($strValidTo);
				if ($timeValidTo != 0)
				{
					$arrayTable[] = array('?TID_SSLCHECK_LABEL_VALIDITY?',RenderDateTime($timeValidTo));
				}	
			}


			// SSL Certificate Details
			if (is_array($arrayCert) == true)
			{
				foreach ($arrayCert as $strKey => $vValue)
				{
					if (is_array($vValue) == true)
					{
						$arraySubTable = array();
						foreach ($vValue as $strSubKey => $vSubValue)
						{
							$arraySubTable[] = array($strSubKey,$vSubValue);
						}
						$vValue = $table->RenderTable($arraySubTable);
					}
					$arrayTable[] = array($strKey,$vValue);
				}
			}
			
		 
			
			//$arrayTable[] = array("1",$certinfo);
			
			// Experiment 2
		
		/*
			
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL,"https://www.digicert.com/");
    //curl_setopt($ch, CURLOPT_STDERR, $fp);
    curl_setopt($ch, CURLOPT_CERTINFO, 1);
    curl_setopt($ch, CURLOPT_VERBOSE, 1);
    curl_setopt($ch, CURLOPT_HEADER, 1);
    curl_setopt($ch, CURLOPT_NOBODY, 1);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, true);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST,  2);
    $result = curl_exec($ch);
    
    */
    
    //curl_errno($ch)==0 or die("Error:".curl_errno($ch)." ".curl_error($ch));


			//$arrayTable[] = array("2",$result);
			


			
			
			
			
			
			
			
						
			
			$this->SetOutputItem( 
				"RESULT",
				$table->RenderTable($arrayTable));
					
			return $this->StateStart();
		}
		

	}

