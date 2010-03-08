/*
Copyright (C) 2005 Parallel Realities

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#define MULTIPLIER 65599
#define MAX_BUCKETS 256

class Hashtable {

	private:

		struct Entry {

			unsigned int hash;

			void *data;

			Entry *previous;
			Entry *next;

		};

		Entry *bucket[MAX_BUCKETS];
		
		List list;

	public:
		
		bool nullWarning;
		
		String name;

	Hashtable();
	~Hashtable();

	void put(char *key, void *data);
	void *get(char *key);
	void remove(char *key);
	
	List *toList();
	
	int getSize();

	void clear();

	void printTable();

};
