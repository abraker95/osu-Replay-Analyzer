#ifndef DATABASE_H
#define DATABASE_H

#include <assert.h>
#include <vector>
#include <iostream>
#include <functional>
#include "EntryDB.h"


template <typename T>
class Database
{
	public:
		Database() {}
		~Database() { Clear(); }

		void Insert(T* _entry, int _val)
		{	
			std::vector<int> idxObj = this->ObjFind(_entry, true);
			std::vector<int> idxVal = this->Find(_val, true);

			EntryDB<T>* entryObj = new EntryDB<T>(_entry, _val);
			EntryDB<T>* entryVal = new EntryDB<T>(_entry, _val);

			if (this->entriesObj.size() == 0) this->entriesObj.push_back(entryObj);
			else                              this->entriesObj.insert(entriesObj.begin() + idxObj[0], entryObj);

			if (this->entriesVal.size() == 0) this->entriesVal.push_back(entryVal);
			else							  this->entriesVal.insert(entriesVal.begin() + idxVal[0], entryVal);
		}

		void Remove(int _val, std::vector<int> _select = std::vector<int>())
		{
			std::vector<int> foundVal = this->Find(_val, true);
			if (_select.size() == 0) _select = foundVal;

			for (int idxVal : foundVal)
			{
				std::vector<int> foundObj = this->ObjFind(entriesVal[idxVal]->getObj(), true);
				if (foundObj.size() > 0)
				{
					for (int idxObj : foundObj)
					{
						delete entriesObj[idxObj]
						entriesObj.erase(entriesObj.begin() + idxObj);
					}
				}

				delete entriesObj[idxObj]
				entriesVal.erase(entriesVal.begin() + idxVal);
			}
		}

		void Remove(T* _obj, std::vector<int> _select = std::vector<int>())
		{
			std::vector<int> foundObj = this->ObjFind(_obj, true);
			if (_select.size() == 0) _select = foundObj;

			for (int idxObj : foundObj)
			{
				std::vector<int> foundVal = this->Find(*entriesObj[idxObj]->getVal(), true);
				if (foundVal.size() > 0)
				{
					for (int idxVal : foundVal)
					{
						delete entriesVal[idxVal]
						entriesVal.erase(entriesVal.begin() + idxVal);
					}
				}

				delete entriesObj[idxObj]
				entriesObj.erase(entriesObj.begin() + idxObj);
			}
		}

		std::vector<int> Find(int _val, bool _nearest)
		{
			int start = 0, end = entriesVal.size(), mid = (start + end) / 2;
			while (start < end)
			{
				int val = *(this->entriesVal[mid]->getVal());
				if (_val == val)
				{
					// See if there are multiple entries of this and compile a list
					while ((_val == *(this->entriesVal[mid]->getVal()))) { if (--mid < 0) break; }

					std::vector<int> found;
					for (int i = (mid < 0)? 0: mid + 1; i < entriesVal.size(); i++)
					{
						if (_val == *this->entriesVal[i]->getVal()) found.push_back(i);
						else break;
					}

					return found;
				}

				if (_val < val) end = mid;
				else            start = mid + 1;

				mid = (start + end) / 2;
			}

			mid = BOUND(0, mid, entriesVal.size() - 1);

			if (_nearest) return std::vector<int>({ mid });
			else          return std::vector<int>();
		}


		int Find(T* _obj, bool _nearest)
		{
			// Find sorted object index
			std::vector<int> foundObj = objFind(_obj, false);
			if (foundObj.size() == 0) return -1;

			// Resolve it to sorted value index
			std::vector<int> foundVal = Find(entriesObj[foundObj[0]]->getVal())
			if (foundVal.size() == 0) return -1;

			return foundVal[0];
		}

		std::vector<int*> Get(T* _obj)
		{
			std::vector<int*> vals;
			std::vector<int> indices = ObjFind(_obj, false);

			if (indices.size() == 0) return vals;

			for (int i : indices)
				vals.push_back(entriesObj[i]->getVal());

			return vals;
		}

		std::vector<T*> Get(int _val)
		{
			std::vector<T*> objs;

			std::vector<int> indices = this->Find(_val, false);
			if (indices.size() == 0) return objs;

			for (int i : indices)
				objs.push_back(entriesVal[i]->getObj());

			return objs;
		}

		int GetIdx(int _idx, int _offset)
		{
			std::vector<int> indices = valFind(entriesObj[_idx]->getVal(), true) + _offset
			return Find(entriesVal[idx]->getObj(), true);
		}

		void Clear()
		{
			assert(entriesObj.size() == entriesVal.size());

			for (EntryDB<T>* entry : entriesObj)
			{
				if (entry != nullptr)
				{
					delete entry;
					entry = nullptr;
				}
			}

			for (EntryDB<T>* entry : entriesVal)
			{
				if (entry != nullptr)
				{
					delete entry;
					entry = nullptr;
				}
			}

			entriesObj.clear();
			entriesVal.clear();

			std::vector<EntryDB<T>*>().swap(entriesObj);
			std::vector<EntryDB<T>*>().swap(entriesVal);
		}

		int size()
		{
			return entriesObj.size();
		}

		T* operator[] (const int _idx)
		{
			if(!BTWN(0, _idx, entriesVal.size() - 1)) return nullptr;
			return entriesVal[_idx]->getObj();
		}

	private:
		std::vector<EntryDB<T>*> entriesObj;
		std::vector<EntryDB<T>*> entriesVal;

		std::vector<int> ObjFind(T* _obj, bool _nearest)
		{
			int start = 0, end = entriesObj.size(), mid = (start + end) / 2;
			while (start < end)
			{
				void* addr = this->entriesObj[mid]->getObjAddr();
				if ((void*)_obj == addr)
				{
					// See if there are multiple entries of this and compile a list
					while ((void*)_obj == this->entriesObj[mid]->getObjAddr()) { if (--mid < 0) break; }

					std::vector<int> found;
					for (int i = (mid < 0)? 0: mid + 1; i < entriesVal.size(); i++)
					{
						if ((void*)_obj == this->entriesObj[i]->getObjAddr()) found.push_back(i);
						else break;
					}

					return found;
				}

				if ((void*)_obj < addr) end = mid;
				else                    start = mid + 1;

				mid = (start + end) / 2;
			}

			mid = BOUND(0, mid, entriesVal.size() - 1);

			if (_nearest) return std::vector<int>({ mid });
			else          return std::vector<int>();
		}
};


#endif // !DATABASE_H