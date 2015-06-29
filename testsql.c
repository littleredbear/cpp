#include <mysql.h>
#include <stdio.h>


int main (int argc, char **argv) 
{
	MYSQL mysql;
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;

	char x_sql[1000]="\0";
	int ID[2];
	char name[2][100];
	char Default[2][108];

	mysql_library_init(0, NULL, NULL);
	
	if (mysql_init(&mysql) == NULL)
	{

	printf("init error\n");

	}
	if (mysql_real_connect(&mysql,"localhost","root","einzweidrei","ungo",3306,NULL,0))
	{
			printf("connected\nclient version: %s\n", mysql_get_client_info());
			int t=mysql_query(&mysql, "select * from bird");
			if (t) {
					printf("query error\n");
			}else {
					res = mysql_use_result(&mysql);
					if(res)
					{
							int fx=mysql_field_count(&mysql);
							printf("fx:%d\n", fx);
							while((row=mysql_fetch_row(res)))
							{
									for(int j=0;j<mysql_num_fields(res);++j)
										printf("%s ",row[j]);
									printf("\n");
							}
					}
			}
	}

	printf("test sql\n");
}
