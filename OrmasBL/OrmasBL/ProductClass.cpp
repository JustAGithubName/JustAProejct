#include "stdafx.h"
#include "ProductClass.h"
#include <boost/algorithm/string.hpp>
#include "PriceClass.h"

namespace BusinessLayer
{
	Product::Product(DataLayer::productsCollection pCollection)
	{
		id = std::get<0>(pCollection);
		companyID = std::get<1>(pCollection);
		name = std::get<2>(pCollection);
		volume = std::get<3>(pCollection);
		measureID = std::get<4>(pCollection);
		price = std::get<5>(pCollection);
		productTypeID = std::get<6>(pCollection);
		shelfLife = std::get<7>(pCollection);
		currencyID = std::get<8>(pCollection);
	}

	int Product::GetID()
	{
		return id;
	}

	int Product::GetCompanyID()
	{
		return companyID;
	}

	std::string Product::GetName()
	{
		return name;
	}

	double Product::GetVolume()
	{
		return volume;
	}

	int Product::GetMeasureID()
	{
		return measureID;
	}

	double Product::GetPrice()
	{
		return price;
	}

	int Product::GetProductTypeID()
	{
		return productTypeID;
	}

	int Product::GetShelfLife()
	{
		return shelfLife;
	}

	int Product::GetCurrencyID()
	{
		return currencyID;
	}

	void Product::SetID(int pID)
	{
		id = pID;
	}
	void Product::SetCompanyID(int pCompanyID)
	{
		companyID = pCompanyID;
	}
	void Product::SetName(std::string pName)
	{
		if (!pName.empty())
			boost::trim(pName);
		name = pName;
	}
	void Product::SetVolume(double pVolume)
	{
		volume = pVolume;
	}
	void Product::SetMeasureID(int pMeasureID)
	{
		measureID = pMeasureID;
	}
	void Product::SetPrice(double pPrice)
	{
		price = pPrice;
	}
	void Product::SetProductTypeID(int pProdTypeID)
	{
		productTypeID = pProdTypeID;
	}
	void Product::SetShelfLife(int pShelfLife)
	{
		shelfLife = pShelfLife;
	}
	void Product::SetCurrencyID(int pCurrencyID)
	{
		currencyID = pCurrencyID;
	}

	bool Product::CreateProduct(DataLayer::OrmasDal& ormasDal, int cID, std::string pName, double vol, int mID, double pri,
		int pTypeID, int pShelfLife, int currID, std::string& errorMessage)
	{
		if (IsDuplicate(ormasDal, cID, pName, vol, mID, price, currID, errorMessage))
			return false;
		id = ormasDal.GenerateID();
		TrimStrings(pName);
		companyID = cID;
		name = pName;
		volume = vol;
		measureID = mID;
		price = pri;
		productTypeID = pTypeID;
		shelfLife = pShelfLife;
		currencyID = currID;
		if (0 != id && ormasDal.CreateProduct(id, companyID, name, volume, measureID, price, productTypeID, shelfLife, currencyID, errorMessage))
		{
			return true;
		}
		return false;
	}
	bool Product::CreateProduct(DataLayer::OrmasDal& ormasDal, std::string& errorMessage)
	{
		if (IsDuplicate(ormasDal, errorMessage))
			return false;
		id = ormasDal.GenerateID();
		if (0 != id && ormasDal.CreateProduct(id, companyID, name, volume, measureID, price, productTypeID, shelfLife, currencyID, errorMessage))
		{
			return true;
		}
		return false;
	}
	bool Product::DeleteProduct(DataLayer::OrmasDal& ormasDal, std::string& errorMessage)
	{
		companyID = 0;
		name.clear();
		volume = 0.0;;
		measureID = 0;
		price =  0.0;
		productTypeID = 0;
		shelfLife = 0;
		if (ormasDal.DeleteProduct(id, errorMessage))
		{
			id = 0;
			return true;
		}
		return false;
	}
	bool Product::UpdateProduct(DataLayer::OrmasDal& ormasDal, int cID, std::string pName, double vol, int mID, double pri,
		int pTypeID, int pShelfLife, int pCurrencyID, std::string& errorMessage)
	{
		TrimStrings(pName);
		companyID = cID;
		name = pName;
		volume = vol;
		measureID = mID;
		price = pri;
		productTypeID = pTypeID;
		shelfLife = pShelfLife;
		currencyID = pCurrencyID;
		if (0 != id && ormasDal.UpdateProduct(id, companyID, name, volume, measureID, price, productTypeID, shelfLife, currencyID, errorMessage))
		{
			return true;
		}
		return false;
	}
	bool Product::UpdateProduct(DataLayer::OrmasDal& ormasDal, std::string& errorMessage)
	{
		if (0 != id && ormasDal.UpdateProduct(id, companyID, name, volume, measureID, price, productTypeID, shelfLife, currencyID, errorMessage))
		{
			return true;
		}
		return false;
	}

	std::string Product::GenerateFilter(DataLayer::OrmasDal& ormasDal)
	{
		if (0 != id || 0 != companyID || !name.empty() || 0 != volume || 0 != measureID || 0 != price || 0 != productTypeID 
			|| 0 != shelfLife || 0 != currencyID)
		{
			return ormasDal.GetFilterForProduct(id, companyID, name, volume, measureID, price, productTypeID, shelfLife, currencyID);
		}
		return "";
	}

	bool Product::GetProductByID(DataLayer::OrmasDal& ormasDal, int pID, std::string& errorMessage)
	{
		id = pID;
		std::string filter = GenerateFilter(ormasDal);
		std::vector<DataLayer::productsViewCollection> productVector = ormasDal.GetProducts(errorMessage, filter);
		if (0 != productVector.size())
		{
			id = std::get<0>(productVector.at(0));
			name = std::get<1>(productVector.at(0));
			price = std::get<2>(productVector.at(0));
			volume = std::get<4>(productVector.at(0));
			shelfLife = std::get<7>(productVector.at(0));
			companyID = std::get<9>(productVector.at(0));
			measureID = std::get<10>(productVector.at(0));
			productTypeID = std::get<11>(productVector.at(0));
			currencyID = std::get<12>(productVector.at(0));
			return true;
		}
		else
		{
			errorMessage = "Cannot find product with this id";
		}
		return false;
	}
	
	bool Product::IsEmpty()
	{
		if(0 == id && name == "" && 0 == price && 0 == volume && 0 == shelfLife && 0 == companyID 
			&& 0 == measureID && 0 == productTypeID && 0 == currencyID)
			return true;
		return false;
	}

	void Product::TrimStrings(std::string& pName)
	{
		if (!pName.empty())
			boost::trim(pName);
	}

	bool Product::IsDuplicate(DataLayer::OrmasDal& ormasDal, int cID, std::string pName, double vol, int mID, double price,
		int curID, std::string& errorMessage)
	{
		Product product;
		product.SetCompanyID(cID);
		product.SetName(pName);
		product.SetVolume(vol);
		product.SetMeasureID(mID);
		product.SetPrice(price);
		product.SetCurrencyID(curID);
		std::string filter = product.GenerateFilter(ormasDal);
		std::vector<DataLayer::productsViewCollection> productVector = ormasDal.GetProducts(errorMessage, filter);
		if (!errorMessage.empty())
			return true;
		if (0 == productVector.size())
		{
			return false;
		}
		errorMessage = "Product with this parameters are already exist! Please avoid the duplication!";
		return true;
	}

	bool Product::IsDuplicate(DataLayer::OrmasDal& ormasDal, std::string& errorMessage)
	{
		Product product;
		product.SetCompanyID(companyID);
		product.SetName(name);
		product.SetVolume(volume);
		product.SetMeasureID(measureID);
		product.SetPrice(price);
		product.SetCurrencyID(currencyID);
		std::string filter = product.GenerateFilter(ormasDal);
		std::vector<DataLayer::productsViewCollection> productVector = ormasDal.GetProducts(errorMessage, filter);
		if (!errorMessage.empty())
			return true;
		if (0 == productVector.size())
		{
			return false;
		}
		errorMessage = "Product with this parameters are already exist! Please avoid the duplication!";
		return true;
	}

	bool Product::AddPriceData(DataLayer::OrmasDal& ormasDal, int pID, double price, int curID, std::string& errorMessage)
	{
		Price pri;
		//To do: impliment this function
		/*price.SetDate();
		price.SetValue(price);
		price.SetCurrencyID(curID);
		price.SetProductID(pID);*/
		return false;
	}
}