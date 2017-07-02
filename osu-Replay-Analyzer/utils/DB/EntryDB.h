#ifndef ENTRYDB_H
#define ENTRYDB_H

template <typename T>
class EntryDB
{
	public:
		bool manage;

		EntryDB(T* _obj, int _val, bool _manage)
		{
			obj = _obj;
			val = new int(_val);
			manage = _manage;
		}

		~EntryDB()
		{
			delete val;

			if(manage)
				delete obj;
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