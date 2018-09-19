<?php
session_start();
require_once 'logsql.php';

$productPrice = Array();
	$product_type_result = pg_query("SELECT product_type_id FROM \"OrmasSchema\".product_types_view where product_type_code = 'PRODUCT'");
    $product_type_row = pg_fetch_array($product_type_result);
	$product_result = pg_query("SELECT * FROM \"OrmasSchema\".products_view where product_type_id = ".$product_type_row[0]);
	$product_row = pg_fetch_all($product_result);
	$product_list ="";
	if(!empty($product_row[0]))
	{
		$row_count = pg_num_rows($product_result);
		for($i=0;$i<$row_count;$i++)
		{
			$product_list .= "<option value='".$product_row[$i]['product_id']."'>".$product_row[$i]['product_name']."</option>";
			$productPrice[$i] = array("id" => $product_row[$i]['product_id'], "price" => $product_row[$i]['price']);
		}
	}
	
$status_result= pg_query("SELECT status_id FROM \"OrmasSchema\".status_view where status_name = 'TO RETURN'");
$status_row = pg_fetch_all($status_result);
$status_id ="";
if(!empty($product_row[0]))
{
	$row_count = pg_num_rows($status_result);
	for($i=0;$i<$row_count;$i++)
	{
		$status_id = $status_row[$i]['status_id'];
	}
}

$currency_result= pg_query("SELECT currency_id FROM \"OrmasSchema\".currencies_view where currency_short_name = 'TJS'");
$currency_row = pg_fetch_all($currency_result);
$currency_id ="";
if(!empty($currency_row[0]))
{
	$row_count = pg_num_rows($currency_result);
	for($i=0;$i<$row_count;$i++)
	{
		$currency_id = $currency_row[$i]['currency_id'];
	}
}

$status_result= pg_query("SELECT \"OrmasSchema\".getcurrentdate()");
$status_row = pg_fetch_row($status_result);
$currentDate = $status_row[0];


if($_SESSION['role_id_client'] == $_SESSION['role_id'])
{
$form_string= "
<div id='form-body'>
	<form method='post' id = 'return-form'>
		<div id='form-title'>
			<label>Создать возврат</label>
		</div>
		<input type='hidden' name='form_type' value='return'/>
		<input type='hidden' name='client' value='".$_SESSION['id']."'/>
		<input type='hidden' name='date' value='".$currentDate."'/>
		<input type='hidden' name='employee' value=''/>
		<input type='hidden' name='status' value='".$status_id."'/>
		<input type='hidden' name='returncount' id ='returncount' value=''/>
		<input type='hidden' name='sum' id='sum' value=''/>
		<input type='hidden' name='currency' value='".$currency_id."'/>
		<input type='hidden' name='prices' value='".json_encode($productPrice)."'/>
		<div id='form-content'>
			<div class='addable'>
				<div class='close-bar'>
					<!--<div class='close-img'>
					</div>-->
				</div>
				<div class='row'>
					<div class='float-left'>
						<label class='float-left form-label'>Виберите продукт:</label>
						<select class='form-prod' name='product[]'>
						".$product_list."
						</select>
					</div>
					<div >
						<label class='float-left form-label'>Количество:</label>
						<input class ='form-count' type='number' name='count[]'/>
					</div>
				</div>
			</div>
		</div>
		<div id='form-button'>
			<div id='form-button-content'>
				<div id='form-button-img' class='float-left'>
				</div>
				<div id='form-buttom-text'>
					Добавить еще...
				</div>
			</div>
		</div>
		<div id='from-submit'>
			<input class='form-submit' type='button' value='Создать' onclick='SubmitReturnForm()' id='sub-button'/>
		<div>
	</form>
</div>";
echo ($form_string);
}
if($_SESSION['role_id_expeditor'] == $_SESSION['role_id'])
{	
$query_client = "SELECT user_id_2 FROM \"OrmasSchema\".relations_view WHERE user_id_1=".$_SESSION['id'];			
$result_clinet = pg_query($query_client);
$client_options = "";
while ($row_user = pg_fetch_array($result_clinet)) {
	$query = "SELECT user_id, user_name, user_surname, user_phone, city_name, user_address, firm	 FROM \"OrmasSchema\".clients_view WHERE user_id=".$row_user[0];			
	$result = pg_query($query);
	while ($row = pg_fetch_array($result)) {
		$client_options .= "<option value='".$row[0]."'>".$row[1].", ".$row[2].", ".$row[3].", ".$row[6]."</option>";
	}
}

$form_string= "
<script>
  $(document).ready(function() { 
            $('#selected-clinet').select2({
                    placeholder: '',
                    allowClear: false
             }); 
			$('.form-prod').select2({
                    placeholder: '',
                    allowClear: false
            }); 
        });
</script>
<div id='form-body'>
	<form method='post' id = 'return-form'>
		<div id='form-title'>
			<label>Создать возврат</label>
		</div>
		<input type='hidden' name='form_type' value='return'/>
		<input type='hidden' name='date' value='".$currentDate."'/>
		<input type='hidden' name='employee' value='".$_SESSION['id']."'/>
		<input type='hidden' name='status' value='".$status_id."'/>
		<input type='hidden' name='returncount' id ='returncount' value=''/>
		<input type='hidden' name='sum' id='sum' value=''/>
		<input type='hidden' name='currency' value='".$currency_id."'/>
		<input type='hidden' name='prices' value='".json_encode($productPrice)."'/>
		<div id='form-content'>
			<div class='row'>
					<div class='float-left'>
						<label class='float-left form-label'>Виберите клиента:</label>
						<select class='form-client' name='selected_clinet' id='selected-clinet'>
						".$client_options."
						</select>
					</div>
			</div>
			<div class='addable'>
				<div class='close-bar'>
					<!--<div class='close-img'>
					</div>-->
				</div>
				
				<div class='row'>
					<div class='float-left'>
						<label class='float-left form-label'>Виберите продукт:</label>
						<select class='form-prod' name='product[]'>
						".$product_list."
						</select>
					</div>
					<div >
						<label class='float-left form-label'>Количество:</label>
						<input class ='form-count' type='number' name='count[]'/>
					</div>
				</div>
			</div>
		</div>
		<div id='form-button'>
			<div id='form-button-content'>
				<div id='form-button-img'>
				</div>
				<div id='form-buttom-text'>
					Добавить еще...
				</div>
			</div>
		</div>
		<div id='from-submit'>
			<input class='form-submit' type='button' value='Создать' onclick='SubmitReturnForm()' id='sub-button'/>
		<div>
	</form>
</div>";
echo ($form_string);
}
?>