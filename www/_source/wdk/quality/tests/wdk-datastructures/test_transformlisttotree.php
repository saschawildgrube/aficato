<?php
	
	require_once(GetWDKDir().'wdk_datastructures.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('TransformListToTree');
		}
		

		function TestCase_TransformListToTree(
			$arrayList,
			$strLevelKey,
			$strChildrenKey,
			$arrayExpectedResult)
		{ 
			$this->Trace('TestCase_TransformListToTree');
	 
			$this->Trace('Input List:');
			$this->Trace($arrayList);

			$this->Trace('Level key   : '.$strLevelKey);
			$this->Trace('Children key: '.$strChildrenKey);


			$this->Trace('Expected:');
			$this->Trace($arrayExpectedResult);


			$arrayResult = TransformListToTree($arrayList,$strLevelKey,$strChildrenKey);
			$this->Trace('TransformListToTree returns:');
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
			$this->TestCase_TransformListToTree($arrayList,'level','children',$arrayExpectedResult);

			$arrayList = array(
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

			$arrayExpectedResult = array(
				array(
					'label' => 'Project',
					'level' => 0,
					'children' => array(
						array(
							'label' => 'Epic Alpha',
							'level' => 1,
							'children' => array(
								array(
									'label' => 'Story A.1',
									'level' => 2),
								array(
									'label' => 'Story A.2',
									'level' => 2)
								)
							),
						array(
							'label' => 'Epic Beta',
							'level' => 1,
							'children' => array(
								array(
									'label' => 'Story B.1',
									'level' => 2),
								array(
									'label' => 'Story B.2',
									'level' => 2)
								)
							)
						) 
					)
				);
			$this->TestCase_TransformListToTree($arrayList,'level','children',$arrayExpectedResult);

		}
		
		
	}
	
	


		
