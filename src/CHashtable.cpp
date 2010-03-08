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

#include "headers.h"

Hashtable::Hashtable()
{
	for (int i = 0 ; i < MAX_BUCKETS ; i++)
	{
		bucket[i] = NULL;
	}
	
	nullWarning = true;
	
	name = "Unnamed Hashtable";
}

Hashtable::~Hashtable()
{
	clear();
}

void Hashtable::put(char *key, void *data)
{
	unsigned int hash = 0;
	unsigned int entryBucket = 0;

	for (unsigned int i = 0 ; i < strlen(key) ; i++)
	{
		hash = hash * MULTIPLIER + key[i];
	}

	entryBucket = hash % MAX_BUCKETS;

	Entry *entry;

	if (bucket[entryBucket] == NULL)
	{
		entry = new Entry();
		entry->hash = hash;
		entry->data = data;
		entry->previous = NULL;
		entry->next = NULL;

		bucket[entryBucket] = entry;

		//printf("Added %s = (Hash = %d : Bucket = %d (1))\n", key, hash, entryBucket);
	}
	else
	{
		Entry *last = bucket[entryBucket];
		int count = 1;

		while (true)
		{
			count++;

			// already added!
			if (last->hash == hash)
			{
				debug(("WARNING - POSSIBLE HASHCODE COLLISION!! - %s\n", key));
				return;
			}

			if (last->next != NULL)
			{
				last = last->next;
			}
			else
			{
				break;
			}
		}

		entry = new Entry();
		entry->hash = hash;
		entry->data = data;
		entry->previous = last;
		entry->next = NULL;

		last->next = entry;

		//printf("Added %s = (Hash = %d : Bucket = %d (%d))\n", key, hash, entryBucket, count);
	}
}

void *Hashtable::get(char *key)
{
	unsigned int hash = 0;
	unsigned int entryBucket = 0;

	for (unsigned int i = 0 ; i < strlen(key) ; i++)
	{
		hash = hash * MULTIPLIER + key[i];
	}

	entryBucket = hash % MAX_BUCKETS;

	Entry *entry = bucket[entryBucket];

	while (entry != NULL)
	{
		if (entry->hash == hash)
		{
			return entry->data;
		}

		entry = entry->next;
	}
	
	if (nullWarning)
	{
		printf("WARNING - Hashtable::get('%s') - Returning NULL!\n", key);
	}

	return NULL;
}

void Hashtable::remove(char *key)
{
	unsigned int hash = 0;
	unsigned int entryBucket = 0;

	for (unsigned int i = 0 ; i < strlen(key) ; i++)
	{
		hash = hash * MULTIPLIER + key[i];
	}

	entryBucket = hash % MAX_BUCKETS;

	Entry *entry = bucket[entryBucket];

	while (entry != NULL)
	{
		if (entry->hash == hash)
		{
			if (entry->previous != NULL)
			{
				entry->previous->next = entry->next;
			}

			if (entry->next != NULL)
			{
				entry->next->previous = entry->previous;
			}

			free(entry->data);
			
			delete entry;

			if (entry == bucket[entryBucket])
			{
				bucket[entryBucket] = NULL;
			}

			return;
		}

		entry = entry->next;
	}
}

int Hashtable::getSize()
{
	int count = 0;
	Entry *entry;

	for (int i = 0 ; i < MAX_BUCKETS ; i++)
	{
		if (bucket[i] != NULL)
		{
			count++;

			entry = bucket[i]->next;

			while (entry != NULL)
			{
				count++;

				entry = entry->next;
			}
		}
	}
	
	return count;
}

List *Hashtable::toList()
{
	list.clear();
	
	Entry *entry;
	Reference *ref;

	for (int i = 0 ; i < MAX_BUCKETS ; i++)
	{
		if (bucket[i] != NULL)
		{
			ref = new Reference();
			ref->object = (GameObject*)bucket[i]->data;
			list.add(ref);
			
			entry = bucket[i]->next;

			while (entry != NULL)
			{
				ref = new Reference();
				ref->object = (GameObject*)entry->data;
				list.add(ref);
				
				entry = entry->next;
			}
		}
	}
	
	return &list;
}

void Hashtable::printTable()
{
	int count = 0;
	Entry *entry;

	for (int i = 0 ; i < MAX_BUCKETS ; i++)
	{
		if (bucket[i] != NULL)
		{
			count++;

			printf("%.3d = %d", i, bucket[i]->hash);

			entry = bucket[i]->next;

			while (entry != NULL)
			{
				count++;

				printf(", %d", entry->hash);

				entry = entry->next;
			}

			printf("\n");
		}
	}

	printf("%d entries in table\n", count);
}

void Hashtable::clear()
{
	Entry *entry = NULL;
	Entry *entry2 = NULL;

	int count = 0;

	for (int i = 0 ; i < MAX_BUCKETS ; i++)
	{
		for (entry = bucket[i] ; entry != NULL ; entry = entry2)
		{
			entry2 = entry->next;
			free(entry->data);
			delete entry;
			count++;
		}
		
		bucket[i] = NULL;
	}

	if ((count > 0) && (nullWarning))
	{
		debug(("Removed %d items from %s Hashtable\n", count, name.getText()));
	}
}
