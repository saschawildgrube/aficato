<?php

	require_once(GetSourceDir().'webservices_directory.inc');

	/*
	
	1. Create a job
	2. Get a Job
	3. Set a Job
	4. Execute a job without forceexecution
	5. Execute a job with forceexecution
	6. Delete a job
	
	*/


	
	class CTest extends CUnitTest
	{
		private $m_strWebservice;
		private $m_strJobID;
		private $m_strJobName;
		
		function __construct()
		{
			$arrayConfig = array();
			$arrayConfig['webservices'] = GetWebServicesDirectory();
			parent::__construct('Web service system/scheduler',$arrayConfig);
		}
		
		function OnInit()
		{
			parent::OnInit();
			
			$this->RequireWebservice('system/scheduler');
			$this->RequireWebservice('system/log');
			
			$this->m_strWebservice = 'system/scheduler';
			$this->m_strJobName = 'Test Job 2';		
				
			$this->SetVerbose(false);
			//$this->SetActive(false);
			
			
			$this->Trace('Find and delete test job if necessary');
			$consumer = new CWebServiceConsumerWebApplication($this);
			$arrayParams = array();
			$arrayParams['trace'] = '1';
			$arrayParams['blocksize'] = '1';
			$arrayParams['offset'] = '0';
			$arrayParams['filter1'] = 'job_name';
			$arrayParams['filter1_operator'] = '=';
			$arrayParams['filter1_value'] = $this->m_strJobName;
			$arrayParams['command'] = 'list';
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() == '')	
			{
				$list = $consumer->GetResultList();
				if (ArrayCount($list) != 0)
				{
					$this->Trace('job named \''.$this->m_strJobName.'\' has been found.');
					$strJobID = $list[0]['JOB_ID'];
					$arrayParams = array();
					$arrayParams['trace'] = '1';
					$arrayParams['job_id'] = $strJobID;
					$arrayParams['command'] = 'delete';
					$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
					if ($consumer->GetError() != '')	
					{
						$this->Trace('An error occured while deleting job id='.$strJobID.'.');
					}						
				}
			}

			return true;
		}
		
		function OnTest()
		{
			parent::OnTest();
			
			$consumer = new CWebServiceConsumerWebApplication($this);
			
			$this->Trace('1. Create a job');
			$arrayParams = array();
			$arrayParams['trace'] = '1';
			$arrayParams['job_name'] = $this->m_strJobName;
			$arrayParams['job_url'] = 'system/scheduler';
			$arrayParams['job_postparams'] = MakeURLParameters(
				array(
					'trace'=>'0',
					'command'=>'selfcheck'
					)
				,"\n");
			$arrayParams['job_active'] = '0';
			$arrayParams['schedule_minute'] = '*';
			$arrayParams['schedule_hour'] = '*';
			$arrayParams['schedule_dayofmonth'] = '*';
			$arrayParams['schedule_dayofweek'] = '*';
			$arrayParams['schedule_month'] = '*';
			$arrayParams['command'] = 'add';
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$this->m_strJobID = $consumer->GetResultValue('NEW_JOB_ID');
			$this->Trace('New job ID: '.$this->m_strJobID);
			if ($consumer->GetError() != '')	
			{
				return;	
			}
		
		
		
			$this->Trace('2. Get a job');
			$arrayParams = array();
			$arrayParams['trace'] = '1';
			$arrayParams['job_id'] = $this->m_strJobID;
			$arrayParams['command'] = 'get';
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != '')	
			{
				return;	
			}
		
		
			$this->Trace('3. Set a job');
			$arrayParams = array();
			$arrayParams['trace'] = '1';
			$arrayParams['job_id'] = $this->m_strJobID;
			$arrayParams['command'] = 'set';
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != '')	
			{
				return;	
			}
		
		
			$this->Trace('4. Execute a job without forceexecution');
			$arrayParams = array();
			$arrayParams['trace'] = '1';
			$arrayParams['job_id'] = $this->m_strJobID;
			$arrayParams['command'] = 'execute';
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != 'JOB_INACTIVE')	
			{
				return;	
			}
		
		
			$this->Trace('5. Execute a job with forceexecution');
			$arrayParams = array();
			$arrayParams['trace'] = '1';
			$arrayParams['job_id'] = $this->m_strJobID;
			$arrayParams['forceexecute'] = '1';
			$arrayParams['command'] = 'execute';
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != '')	
			{
				return;	
			}
		
		


			$this->SetResult(true);
		}
		
		function OnCleanup()
		{
			parent::OnCleanup();
			
			$consumer = new CWebServiceConsumerWebApplication($this);
			
			$this->Trace('Delete the test job');
			$arrayParams = array();
			$arrayParams['trace'] = '1';
			$arrayParams['job_id'] = $this->m_strJobID;
			$arrayParams['command'] = 'delete';
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != '')	
			{
				return false;	
			}		
			
			return true;
		}
		
		
	}
	
	

		
