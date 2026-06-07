<?php
	
	require_once(GetWDKDir().'wdk_datastructures.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('AlignListLevels');
		}
		

		function TestCase_AlignListLevels(
			$arrayList,
			$strLevelKey,
			$arrayExpectedResult)
		{ 
			$this->Trace('TestCase_AlignListLevels');
	 
			$this->Trace('Input List:');
			$this->Trace($arrayList);

			$this->Trace('Level key   : '.$strLevelKey);

			$this->Trace('Expected:');
			$this->Trace($arrayExpectedResult);


			$arrayResult = AlignListLevels($arrayList,$strLevelKey);
			$this->Trace('AlignListLevels returns:');
			$this->Trace($arrayResult);
			
			if ($arrayResult != $arrayExpectedResult)
			{
				$this->Trace('Expected result does not match.');	
				$this->Trace('Testcase FAILED!');	
				$this->SetResult(false);	
				$this->Trace('');
				$this->Trace('');
				return;
			}
			
			$this->Trace('Testcase PASSED!');
			$this->Trace('');
			$this->Trace('');
		}


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);

			$arrayList = false;
			$arrayExpectedResult = array();
			$this->TestCase_AlignListLevels($arrayList,'level',$arrayExpectedResult);

			$arrayList = array(
				array(
					'label' => 'Project',
					'level' => 0),
				array(
					'label' => 'Epic Alpha',
					'level' => 3),
				array(
					'label' => 'Story A.1',
					'level' => 4),
				array(
					'label' => 'Story A.2',
					'level' => 4),
				array(
					'label' => 'Epic Beta',
					'level' => 1),
				array(
					'label' => 'Story B.1',
					'level' => 3),
				array(
					'label' => 'Story B.2',
					'level' => 2)
				);

			$arrayExpectedResult = array(
				array(
					'label' => 'Project',
					'level' => 0),
				array(
					'label' => 'Epic Alpha',
					'level' => 1),
				array(
					'label' => 'Story A.1',
					'level' => 2),
				array(
					'label' => 'Story A.2',
					'level' => 2),
				array(
					'label' => 'Epic Beta',
					'level' => 1),
				array(
					'label' => 'Story B.1',
					'level' => 2),
				array(
					'label' => 'Story B.2',
					'level' => 2)
				);
			$this->TestCase_AlignListLevels($arrayList,'level',$arrayExpectedResult);



			$arrayList = array(
				array(
					'label' => 'Project 1',
					'level' => 1),
				array(
					'label' => 'Epic Alpha',
					'level' => 10),
				array(
					'label' => 'Story A.1',
					'level' => 11),
				array(
					'label' => 'Story A.2',
					'level' => 11),
				array(
					'label' => 'Epic Beta',
					'level' => 10),
				array(
					'label' => 'Story B.1',
					'level' => 11),
				array(
					'label' => 'Story B.2',
					'level' => 11),
				array(
					'label' => 'Project 2',
					'level' => 1),
				array(
					'label' => 'Epic Alpha',
					'level' => 10),
				array(
					'label' => 'Story A.1',
					'level' => 11),
				array(
					'label' => 'Story A.2',
					'level' => 11),
				array(
					'label' => 'Epic Beta',
					'level' => 10),
				array(
					'label' => 'Story B.1',
					'level' => 11),
				array(
					'label' => 'Story B.2',
					'level' => 11)				);

			$arrayExpectedResult = array(
				array(
					'label' => 'Project 1',
					'level' => 0),
				array(
					'label' => 'Epic Alpha',
					'level' => 1),
				array(
					'label' => 'Story A.1',
					'level' => 2),
				array(
					'label' => 'Story A.2',
					'level' => 2),
				array(
					'label' => 'Epic Beta',
					'level' => 1),
				array(
					'label' => 'Story B.1',
					'level' => 2),
				array(
					'label' => 'Story B.2',
					'level' => 2),
				array(
					'label' => 'Project 2',
					'level' => 0),
				array(
					'label' => 'Epic Alpha',
					'level' => 1),
				array(
					'label' => 'Story A.1',
					'level' => 2),
				array(
					'label' => 'Story A.2',
					'level' => 2),
				array(
					'label' => 'Epic Beta',
					'level' => 1),
				array(
					'label' => 'Story B.1',
					'level' => 2),
				array(
					'label' => 'Story B.2',
					'level' => 2)
				);
			$this->TestCase_AlignListLevels($arrayList,'level',$arrayExpectedResult);



		}
		
		
	}
	
	


		
