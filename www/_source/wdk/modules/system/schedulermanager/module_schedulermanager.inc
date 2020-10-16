<?php

	require_once(GetWDKDir().'wdk_module_entitymanager.inc');
	require_once(GetWDKDir().'wdk_crontab.inc');
	require_once(GetWDKDir().'wdk_entity_job.inc');

	class CSchedulerManagerModule extends CEntityManagerModule
	{
	
		function __construct(
			CWebSite $website,
			$strInstanceID,
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				'system/schedulermanager',
				$strInstanceID,
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			$arrayEntityDefinitions = array(
				'job' => GetEntityDefinitionJob()
				);

			$this->SetEntityDefinitions($arrayEntityDefinitions);

			$this->IncludeElement('format');
			
			return parent::CallbackInit();
		}

		function CallbackRenderModule($strState)
		{
			switch ($strState)
			{
				case '':
					return $this->StateList('job');
				case 'execute':
					return $this->StateExecute();
				default:
					return parent::CallbackRenderModule($strState);
			}
		}
		
		function CallbackCheckInputValues($strEntity,$bAdd,$arrayItem)
		{
			$arrayErrors = parent::CallbackCheckInputValues($strEntity,$bAdd,$arrayItem);
			if ($strEntity == 'job')
			{
				if ($arrayItem['job_name'] == '')
				{
					$arrayErrors['job_name'] = '?TID_JOB_ERROR_JOB_NAMEMISSING?';
				}
				if (IsValidURL($arrayItem['job_url']) == false)
				{
					if (IsValidWebserviceName($arrayItem['job_url'] == false))
					{
						//$strWebserviceURL = $this->GetWebserviceURL($arrayItem['job_url'])
						$arrayErrors['job_url'] = '?TID_JOB_ERROR_FORM_INVALID_URL?';
					}
				}
 
				$arrayItem['job_postparams'] = ReplaceString($arrayItem['job_postparams'],' ','%20');

				if (CrontabParseDefinition($arrayItem['schedule_minute'],0,59) == false)
				{
					$arrayErrors['schedule_minute'] = '?TID_JOB_ERROR_CRONTABINVALID?';
				}
				if (CrontabParseDefinition($arrayItem['schedule_hour'],0,23) == false)
				{
					$arrayErrors['schedule_hour'] = '?TID_JOB_ERROR_CRONTABINVALID?';
				}
				if (CrontabParseDefinition($arrayItem['schedule_dayofmonth'],1,31) == false)
				{
					$arrayErrors['schedule_dayofmonth'] = '?TID_JOB_ERROR_CRONTABINVALID?';
				}
				if (CrontabParseDefinition($arrayItem['schedule_month'],1,12) == false)
				{
					$arrayErrors['schedule_month'] = '?TID_JOB_ERROR_CRONTABINVALID?';
				}
				if (CrontabParseDefinition($arrayItem['schedule_dayofweek'],0,6) == false)
				{
					$arrayErrors['schedule_dayofweek'] = '?TID_JOB_ERROR_CRONTABINVALID?';
				}
			}  
			return $arrayErrors;
			
		}

		function CallbackPrepareItemDataView($strEntity,$strIndex,&$arrayItem)
		{
			$bActive = true;
			if ($strEntity == 'job')
			{
				$bActive = GetBoolValue($arrayItem['job_active']);
				
				$nTimeout = intval(ArrayGetValue($arrayItem,'timeout_seconds'));
				if ($nTimeout == 0)
				{
					$arrayItem['timeout_seconds'] = '?TID_JOB_NOTIMEOUT?';
				}
				
				$strLastExecution = $arrayItem['lastexecution_datetime'];
				if ($strLastExecution == '0000-00-00 00:00:00')
				{
					$arrayItem['lastexecution_datetime'] = '?TID_JOB_JOBWASNEVEREXECUTED?';
				}
				
				$strJobURL = ArrayGetValue($arrayItem,'job_url');
				if (IsValidURL($strJobURL) == true)
				{
					$linkicon = new CLinkIconElement($this->GetWebsite(),'default');
					$arrayItem['job_url'] = $linkicon->RenderLinkIcon($strJobURL,'url',$strJobURL,true);
				}
				else
				{
					//$arrayItem['job_url'] = '';
				}
				
				
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = 'execute';
				$arrayParams[$this->GetModuleVarName('job_id')] = $strIndex;
				$strLinkExecute = $this->GetWebsite()->MakeURL($arrayParams);

				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = 'execute';
				$arrayParams[$this->GetModuleVarName('job_id')] = $strIndex;
				$arrayParams[$this->GetModuleVarName('execute')] = '1';
				$strLinkForceExecute = $this->GetWebsite()->MakeURL($arrayParams);

				$strTasks = ArrayGetValue($arrayItem,'TASKS');
				$linkicon = new CLinkIconElement($this->GetWebsite(),'default');
				$strTasks .= $linkicon->RenderLinkIcon($strLinkExecute,'execute','?TID_JOB_TOOLTIP_EXECUTE?');
				$strTasks .= $linkicon->RenderLinkIcon($strLinkForceExecute,'execute','?TID_JOB_TOOLTIP_FORCEEXECUTE?');
				$arrayItem['TASKS'] = $strTasks;
			}
	
			
			parent::CallbackPrepareItemDataView($strEntity,$strIndex,$arrayItem);
			if ($strEntity == 'job')
			{			
				if ($bActive == false)
				{
					$format = new CFormatElement($this->GetWebsite(),'disabled');
					foreach ($arrayItem as $strAttribute => &$strValue)
					{
						if ($strAttribute != 'INDEX')
						{
							$strValue = $format->RenderFormat($strValue);
						}
					}						
				}
			}
		} 
		
		function CallbackPrepareItemDataModify($strEntity,$strIndex,&$arrayItem)
		{
			if ($strEntity == 'job')
			{			
				$arrayItem['job_postparams'] = ReplaceString(ArrayGetValue($arrayItem,'job_postparams'),'%20',' ');
			}
			parent::CallbackPrepareItemDataModify($strEntity,$strIndex,$arrayItem);
		}
		
		function StateExecute()
		{
			$strEntity = 'job';
			$this->SetState('execute');
			
			$strEntityIndex = $this->GetModuleVar('job_id');
			if ($strEntityIndex == '')
			{
				return $this->StateList('job');	
			}
			$strForceExecute = $this->GetModuleVar('forceexecute');
			
			$arrayEntity = $this->GetEntityDefinition($strEntity);			
			$strWebservice = ArrayGetValue($arrayEntity,'webservice','name');
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());
			$arrayParams = array();
			$arrayParams[$this->GetItemIndexPropertyWebserviceParamName($strEntity)] = $strEntityIndex;
			$arrayParams['command'] = $this->GetItemWebserviceCommand($strEntity,'get');
			$webservice->ConsumeWebService($strWebservice,$arrayParams);
			$strError = $webservice->GetError();
			if ($strError != '')
			{
				$this->SetErrorMessageCodeEntity('job',$strError);
				return $this->StateList($strEntity);
			}
			$arrayItem = $this->GetItemValuesFromWebservice($strEntity,$webservice);
			$strUserFriendly = $arrayItem[ArrayGetValue($arrayEntity,'userfriendlyattribute')];
			$this->GetWebsite()->SetOutputItem(StringUpperCase($strEntity).'_USERFRIENDLY',$strUserFriendly);
			$this->GetWebsite()->SetOutputItem(StringUpperCase($strEntity).'_INDEX',$strEntityIndex);
						
			$arrayParams = array();
			$arrayParams['job_id'] = $strEntityIndex;
			if ($strForceExecute != '')
			{
				$arrayParams['forceexecute'] = $strForceExecute;
			}
			$arrayParams['command'] = 'execute';
			$webservice->ConsumeWebService($strWebservice,$arrayParams);
			$strError = $webservice->GetError();
			if ($strError == '')
			{
				$this->SetInfoMessageCodeEntity('job','EXECUTE_SUCCESS');
				$this->Trace($webservice->GetServiceOutput());
			}
			else
			{
				$this->SetErrorMessageCodeEntity('job',$strError);	
			}
				
			$this->SetModuleVar('state','');
			$this->SetModuleVar('job_id','');
			$this->SetModuleVar('forceexecute','');
			
			return $this->StateList('job');
		}
		
		
	}




