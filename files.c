// Just including mah pretty header that contains my program's functionality skeleton. (:
#include "files.h"

// Create Files struct *ptr creation for dynamic accessibilty
struct Files *Files_Create(char *input_source, char *output_source, size_t line_size, int case_ignore)
{	
	// Just allocate some memory for that struct that we will free when our program finishes its job.
    struct Files *m_File = (struct Files*)malloc(sizeof(struct Files));
	
	// I don't feel like I should comment these... so I'll move on
    m_File->input_source = input_source;
	m_File->output_source = output_source;
    m_File->line_size = line_size;
	m_File->case_ignore = case_ignore;
	
	// We just return the created structure w/allocated memory for it
    return m_File;
};

// Destroy Files pointer and free allocated memory
void Files_Destroy(struct Files *m_File)
{
    // Free heap memory (:
    free(m_File);
	m_File = 0;
}

// Simply close a file stream! :))
void Close(FILE *fh)
{	
	fclose(fh);
	fh = 0;
}

// Read from stdin
void Read_Std(char *line, size_t sz)
{		
	printf("\nEnter here: ");
	fgets(line, sz, stdin);
}

// Write to stdout
void Write_Std(char *line)
{
	printf("\nSuch output source doesn't exist. Sorry mate. :)\nWe will try to handle this situation in a few.\n");
	printf("\nBut hey, that's your outcome, pal: %s\n", line);
	printf("-----------------------------\n");
}

// Write line(s) to file
void Write_File(FILE *ofh, char *line)
{
	// Writefile...	
	fprintf(ofh, "%s", line); // "a+" should automatically put a new line in the end and append the string on it.
}

// Search the file for the string entered in the console program's arguments
void Search_File(FILE *ifh, FILE *ofh, struct Files *m_File, char *line, char *word, char *str, size_t str_size, int case_ignore)
{
	// Read the file line by line (untill it reaches max chars for line_size)	
	
	int line_num = 1; // return the number of the line where a string is found
	int find_result = 0; // return the amount of results found for the searched string
	int is_saved = 0; // used as a bool... if is_saved = 1 -> inform via printing to stdout
	
	
	printf("-----------------------------\n");
	printf("I just started, pal! (:\n");
	printf("Type help/HELP in the console for a guide on how to use the program. (:\n");
	printf("Type clear/CLEAR in order to clean the console. (:\n\n\n");
	
	// if any value for input source file
	if(ifh)
	{
		while(fgets(line, m_File->line_size, ifh) != NULL)
		{
			// this is just a result of strstr (the function returns a char pointer)
			char *strings_equal = (char*)malloc(m_File->line_size);
			strings_equal = NULL;
			
			// that integer we use as boolean by the way cuz' I don't wanna botter including additional lib for that...
			// if it equals 1 it means we wanna ignore any type of case differences -> so we just set them all to lower case
			// otherwise we Do care!
			if(case_ignore == 1)
			{
				char *tempLine = (char*)malloc(m_File->line_size);
				char *tempWord = (char*)malloc(m_File->line_size);

				// making deep copies of the pointer objects line(file line) and word(search string)
				for(int i = 0; i < m_File->line_size; i++)
				{
					tempLine[i] = line[i];
					tempWord[i] = word[i];
				}
				
				// setting them to lower case so I can easily compare them without worrying about case sensitivy
				tempLine = strlwr(tempLine);
				tempWord = strlwr(tempWord);
				
				// setting its value to lowercased line and word
				strings_equal = strstr(tempLine, tempWord);
				
				// Freeing the memory allocated for the temps and setting them to null/0
				free(tempLine);
				free(tempWord);
				tempLine = 0;
				tempWord = 0;
			}
			else 
				strings_equal = strstr(line, word);	// unchanged/original values
			
			// compare file lines with the string we've entered
			if(strings_equal != NULL)
			{
				printf("A match for the string \"%s\" was found on line %d\nHere's it: %s\n", word, line_num, line);
				
				// Write line to output file source
				if(ofh)
				{
					Write_File(ofh, line);
					is_saved = 1;
				}
				// Write line to stdout
				else
					Write_Std(line);
				
				find_result++;
			}
			// increment the line number so we continue with the search until the END. (:
			line_num++;
		}
		
		// Just for design matters and informativity(if such word exists :D)
		if(find_result == 0)
		{
			printf("\nSorry, couldn't find a match for \"%s\" but you can always try again. (:.\n", word);
			printf("-----------------------------\n");
		} 
		printf("\nTotal matches found: %d\n", find_result);
	}
	// If no input -> check again for any output source -> if not any as well -> read from stdin AND write to stdout -> Be happy :)
	else
	{
		// Write what's read from stdin to output file source
		if(ofh)
		{
			printf("Such input source doesn't exist. Sorry mate. :)\nBut you can enter now what you wanna add in the output source.\n");
			Read_Std(str, str_size);
			if(strstr(str, word) != NULL)
				Write_File(ofh, str);
			else
				printf("\This isn't the right word, pal.\nI mean, it isn't the one you've searched for just a few ago.. :/");
			is_saved = 1;
		}
		// Read from stdin AND write to stdout
		else
		{
			printf("Neither the input nor the output source exist. Sorry mate. :)\nBut you can type what you wanna display.\n");
			Read_Std(str, str_size);
			if(strstr(str, word) != NULL)
				Write_Std(str);
			else
				printf("\This isn't the right word, pal.\nI mean, it isn't the one you've searched for just a few ago.. :/");
		}
	}
	
	// Just show user that you've saved the found string in an output file
	if(is_saved == 1)
	{
		printf("\n(Saved in output file)\n");
		printf("-----------------------------\n");
	}
}

// Open and read the file -> Ouput Lines
void Operate_Files_Actions(struct Files *m_File, char *word)
{
	//  Define file sources to read from and write to
    FILE *ifh = fopen(m_File->input_source, "r");; // Open input source
	FILE *ofh = fopen(m_File->output_source, "a+");; // Open output source
	
	// Define some size that's gonna be used in memory alloc for the word we search for
	size_t sz = 256;
	
	char *str = (char*)malloc(sizeof(char) * sz);
	char *line = (char*)malloc(sizeof(char) * m_File->line_size);
	
	Search_File(ifh, ofh, m_File, line, word, str, sz, m_File->case_ignore);
	
	// Free the stuff as usual... (:
	// Ok, that's not a legit second part of the above comment.
	// Do free the memory allocated for every file's line and that for the string we are searching for!
	free(line);
	free(str);
	// Get rid of dangling pointers... :D
	line = 0;
	str = 0;
	
	// Check if any of the files is still opened -> CLOSE();
	if(ifh)
		Close(ifh);
	if(ofh)
		Close(ofh);
}