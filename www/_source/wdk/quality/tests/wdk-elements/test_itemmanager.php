<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Element ItemManager");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit(); 
			//$this->SetActive(false);
			return true;
		}
		
		function TestCase_ItemManagerElement(
			$strURL,
			$strExpected)
		{ 
			$this->Trace("");
			$this->Trace("TestCase_ItemManagerElement"); 
			$this->TestCase_CheckURL($strURL,array($strExpected));
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			$this->SetResult(true);






			$strExpected = '<div>
<table><tbody>
<tr>
<td>FIRST_ACTIVE</td>
<td>ACTIVE:All</td><td>ACTIVE_TO_INACTIVE</td>
<td>INACTIVE:<a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_filteroptions_tab=even">Even</a>
</td><td>INACTIVE_TO_INACTIVE</td>
<td>INACTIVE:<a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_filteroptions_tab=uneven">Uneven</a>
</td><td>LAST_INACTIVE</td>
<td>NO_TAB</td>
</tr></tbody></table>
<ul class="pagination">
<li class="active"><a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_offset=0">1</a></li>
<li><a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_offset=1">2</a></li>
<li><a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_offset=2">3</a></li>
<li><a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_offset=3">4</a></li>
<li><a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_offset=4">5</a></li>
<li><a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_offset=1"><i class="fa fa-chevron-right fa-fw" aria-hidden="true"></i></a></li>
<li><a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_offset=5"><i class="fa fa-forward fa-fw" aria-hidden="true"></i></a></li>
<li><a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_offset=9"><i class="fa fa-fast-forward fa-fw" aria-hidden="true"></i></a></li>
</ul>
<table><tbody><tr>
<td>FIRST_ACTIVE</td>
<td>ACTIVE:Primary</td><td>ACTIVE_TO_INACTIVE</td>
<td>INACTIVE:<a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_columns_tab=secondary">Secondary</a>
</td><td>INACTIVE_TO_INACTIVE</td>
<td>INACTIVE:<a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_columns_tab=tertiary">Tertiary</a>
</td><td>LAST_INACTIVE</td>
<td>NO_TAB</td>
</tr></tbody></table>
<table><tbody>
<tr>
<th>
<span><a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_sort_option=col1&amp;itemmanager_sort_order=asc" title="Ascending sorting"><i class="fa fa-chevron-down fa-fw" aria-hidden="true"></i></a>
<a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_sort_option=col1&amp;itemmanager_sort_order=desc" title="Descending sorting"><i class="fa fa-chevron-up fa-fw" aria-hidden="true"></i></a>
</span>
Column 1<br/><span><a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_sort_option=col2&amp;itemmanager_sort_order=asc" title="Ascending sorting"><i class="fa fa-chevron-down fa-fw" aria-hidden="true"></i></a>
<a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_sort_option=col2&amp;itemmanager_sort_order=desc" title="Descending sorting"><i class="fa fa-chevron-up fa-fw" aria-hidden="true"></i></a>
</span>
Column 2</th>
<th>
Column 3</th>
<th>
Column 4<br/>Column 5</th>
</tr>
<tr>
<td>Row0 Col1<br/>Row0 Col2</td>
<td>Row0 Col3</td>
<td>Row0 Col4<br/>Row0 Col5</td>
</tr>
<tr>
<td>Row1 Col1<br/>Row1 Col2</td>
<td>Row1 Col3</td>
<td>Row1 Col4<br/>Row1 Col5</td>
</tr>
<tr>
<td>Row2 Col1<br/>Row2 Col2</td>
<td>Row2 Col3</td>
<td>Row2 Col4<br/>Row2 Col5</td>
</tr>
<tr>
<td>Row3 Col1<br/>Row3 Col2</td>
<td>Row3 Col3</td>
<td>Row3 Col4<br/>Row3 Col5</td>
</tr>
<tr>
<td>Row4 Col1<br/>Row4 Col2</td>
<td>Row4 Col3</td>
<td>Row4 Col4<br/>Row4 Col5</td>
</tr>
<tr>
<td>Row5 Col1<br/>Row5 Col2</td>
<td>Row5 Col3</td>
<td>Row5 Col4<br/>Row5 Col5</td>
</tr>
<tr>
<td>Row6 Col1<br/>Row6 Col2</td>
<td>Row6 Col3</td>
<td>Row6 Col4<br/>Row6 Col5</td>
</tr>
<tr>
<td>Row7 Col1<br/>Row7 Col2</td>
<td>Row7 Col3</td>
<td>Row7 Col4<br/>Row7 Col5</td>
</tr>
<tr>
<td>Row8 Col1<br/>Row8 Col2</td>
<td>Row8 Col3</td>
<td>Row8 Col4<br/>Row8 Col5</td>
</tr>
<tr>
<td>Row9 Col1<br/>Row9 Col2</td>
<td>Row9 Col3</td>
<td>Row9 Col4<br/>Row9 Col5</td>
</tr>
</tbody></table>
<ul class="pagination">
<li class="active"><a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_offset=0">1</a></li>
<li><a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_offset=1">2</a></li>
<li><a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_offset=2">3</a></li>
<li><a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_offset=3">4</a></li>
<li><a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_offset=4">5</a></li>
<li><a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_offset=1"><i class="fa fa-chevron-right fa-fw" aria-hidden="true"></i></a></li>
<li><a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_offset=5"><i class="fa fa-forward fa-fw" aria-hidden="true"></i></a></li>
<li><a href="http://'.GetRootURL().'quality/testwebsite/en/test-element-itemmanager/?itemmanager_offset=9"><i class="fa fa-fast-forward fa-fw" aria-hidden="true"></i></a></li>
</ul>
</div>';

			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-element-itemmanager";
				$this->TestCase_ItemManagerElement(
				$strURL,
				$strExpected);
			

		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			return true;
		}
		 
		
	} 
			
