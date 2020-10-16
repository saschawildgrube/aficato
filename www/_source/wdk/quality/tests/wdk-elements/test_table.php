<?php
	

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Element Table");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit(); 
			$this->SetResult(true);
			return true;
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();

			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-element-table";

			$this->TestCase_CheckURL(
				$strURL,
				array(
					'<table><tbody>
<tr>
<th>Row0 Col0</th>
<th>Row0 Col1</th>
<th>Row0 Col2</th>
<th>Row0 Col3</th>
<th>Row0 Col4</th>
</tr>
<tr>
<th>Row1 Col0</th>
<td>Row1 Col1</td>
<td>Row1 Col2</td>
<td>Row1 Col3</td>
<td>Row1 Col4</td>
</tr>
<tr>
<th>Row2 Col0</th>
<td>Row2 Col1</td>
<td>Row2 Col2</td>
<td>Row2 Col3</td>
<td>Row2 Col4</td>
</tr>
<tr>
<th>Row3 Col0</th>
<td>Row3 Col1</td>
<td>Row3 Col2</td>
<td>Row3 Col3</td>
<td>Row3 Col4</td>
</tr>
<tr>
<th>Row4 Col0</th>
<td>Row4 Col1</td>
<td>Row4 Col2</td>
<td>Row4 Col3</td>
<td>Row4 Col4</td>
</tr>
<tr>
<th>Row5 Col0</th>
<td>Row5 Col1</td>
<td>Row5 Col2</td>
<td>Row5 Col3</td>
<td>Row5 Col4</td>
</tr>
<tr>
<th>Row6 Col0</th>
<td>Row6 Col1</td>
<td>Row6 Col2</td>
<td>Row6 Col3</td>
<td>Row6 Col4</td>
</tr>
<tr>
<th>Row7 Col0</th>
<td>Row7 Col1</td>
<td>Row7 Col2</td>
<td>Row7 Col3</td>
<td>Row7 Col4</td>
</tr>
<tr>
<th>Row8 Col0</th>
<td>Row8 Col1</td>
<td>Row8 Col2</td>
<td>Row8 Col3</td>
<td>Row8 Col4</td>
</tr>
<tr>
<th>Row9 Col0</th>
<td>Row9 Col1</td>
<td>Row9 Col2</td>
<td>Row9 Col3</td>
<td>Row9 Col4</td>
</tr>
</tbody></table>',
'<table><tbody>
<tr>
<th>Row0 Col0</th>
<td>Row0 Col1</td>
<td>Row0 Col2</td>
<td>Row0 Col3</td>
<td style="text-align: right"
>Row0 Col4</td>
</tr>
<tr>
<td>Row1 Col0</td>
<th>Row1 Col1</th>
<td>Row1 Col2</td>
<td>Row1 Col3</td>
<td>Row1 Col4</td>
</tr>
<tr>
<th>Row2 Col0</th>
<td>Row2 Col1</td>
<td>Row2 Col2</td>
<td>Row2 Col3</td>
<td>Row2 Col4</td>
</tr>
<tr>
<td>Row3 Col0</td>
<th style="text-align: center;">Row3 Col1</th>
<td style="text-align: center
">Row3 Col2</td>
<td style="text-align: center
">Row3 Col3</td>
<td style="text-align: right"
>Row3 Col4</td>
</tr>
<tr>
<th>Row4 Col0</th>
<th>Row4 Col1</th>
<th>Row4 Col2</th>
<th>Row4 Col3</th>
<th>Row4 Col4</th>
</tr>
<tr>
<td>Row5 Col0</td>
<td>Row5 Col1</td>
<td>Row5 Col2</td>
<td>Row5 Col3</td>
<td>Row5 Col4</td>
</tr>
<tr>
<td>Row6 Col0</td>
<td>Row6 Col1</td>
<td>Row6 Col2</td>
<td>Row6 Col3</td>
<th>Row6 Col4</th>
</tr>
<tr>
<td>Row7 Col0</td>
<td>Row7 Col1</td>
<td>Row7 Col2</td>
<td>Row7 Col3</td>
<td>Row7 Col4</td>
</tr>
<tr>
<td>Row8 Col0</td>
<td>Row8 Col1</td>
<td>Row8 Col2</td>
<td>Row8 Col3</td>
<td>Row8 Col4</td>
</tr>
<tr>
<td>Row9 Col0</td>
<td>Row9 Col1</td>
<td>Row9 Col2</td>
<td>Row9 Col3</td>
<td>Row9 Col4</td>
</tr>
</tbody></table>'));
		}
		

		
	}
	
	
		


		
