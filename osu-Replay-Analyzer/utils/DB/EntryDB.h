#ifndef ENTRYDB_H
#define ENTRYDB_H

template <typename T>
class EntryDB
{
	public:
		EntryDB(T* _obj, int _val)
		{
			obj = _obj;
			val = new int(_val);
		}

		~EntryDB()
		{
			if (val != nullptr)
			{
				delete val;
				val = nullptr;
			}
		}

		T* getObj()
		{
			return obj;		
		}

		void* getObjAddr()
		{
			return obj;
		}

		int* getVal()
		{
			return val;
		}

	private:
		T* obj;
		int* val;
};

#endif