#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_COLUMN_NAME 50
#define MAX_TABLE_NAME 50

// 定义列的元数据
typedef struct {
    char column_name[MAX_COLUMN_NAME];
    char data_type[20];  // 如 int、varchar
} Column;

// 表的元数据
typedef struct {
    int num_columns;     // 列数
    Column *columns;     // 列数组
} TableMeta;

// 创建数据库
void create_database(const char *db_name) {
    if (mkdir(db_name, 0777) == -1) {
        perror("创建数据库失败");
    } else {
        printf("数据库 %s 创建成功！\n", db_name);
    }
}

// 删除数据库
void delete_database(const char *db_name) {
    char command[100];
    snprintf(command, sizeof(command), "rm -rf %s", db_name);
    if (system(command) == -1) {
        perror("删除数据库失败");
    } else {
        printf("数据库 %s 删除成功！\n", db_name);
    }
}

// 创建表
void create_table(const char *db_name, const char *table_name, TableMeta *meta) {
    char table_file[100];
    snprintf(table_file, sizeof(table_file), "%s/%s.dat", db_name, table_name);
    FILE *file = fopen(table_file, "wb");

    if (!file) {
        perror("创建表失败");
        return;
    }

    // 写入表的元数据
    fwrite(&meta->num_columns, sizeof(int), 1, file);
    fwrite(meta->columns, sizeof(Column), meta->num_columns, file);
    fclose(file);

    printf("表 %s 创建成功！\n", table_name);
}

// 删除表
void delete_table(const char *db_name, const char *table_name) {
    char table_file[100];
    snprintf(table_file, sizeof(table_file), "%s/%s.dat", db_name, table_name);
    
    if (remove(table_file) == 0) {
        printf("表 %s 删除成功！\n", table_name);
    } else {
        perror("删除表失败");
    }
}
// 插入数据到表
void insert_data(const char *db_name, const char *table_name, void **data) {
    char table_file[100];
    snprintf(table_file, sizeof(table_file), "%s/%s.dat", db_name, table_name);
    FILE *file = fopen(table_file, "ab");

    if (!file) {
        perror("打开表文件失败");
        return;
    }

    TableMeta meta;
    fread(&meta.num_columns, sizeof(int), 1, file);  // 读取元数据
    meta.columns = malloc(meta.num_columns * sizeof(Column));
    fread(meta.columns, sizeof(Column), meta.num_columns, file);

    // 写入数据
    for (int i = 0; i < meta.num_columns; i++) {
        if (strcmp(meta.columns[i].data_type, "int") == 0) {
            fwrite(data[i], sizeof(int), 1, file);
        } else if (strcmp(meta.columns[i].data_type, "varchar") == 0) {
            fwrite(data[i], sizeof(char), strlen((char *)data[i]) + 1, file);
        }
    }

    fclose(file);
    free(meta.columns);
    printf("数据插入成功！\n");
}

// 查询数据
void query_data(const char *db_name, const char *table_name) {
    char table_file[100];
    snprintf(table_file, sizeof(table_file), "%s/%s.dat", db_name, table_name);
    FILE *file = fopen(table_file, "rb");

    if (!file) {
        perror("打开表文件失败");
        return;
    }

    TableMeta meta;
    fread(&meta.num_columns, sizeof(int), 1, file);  // 读取元数据
    meta.columns = malloc(meta.num_columns * sizeof(Column));
    fread(meta.columns, sizeof(Column), meta.num_columns, file);

    // 查询数据
    void *row = malloc(meta.num_columns * sizeof(void *));
    while (fread(row, sizeof(void *), meta.num_columns, file) == meta.num_columns) {
        for (int i = 0; i < meta.num_columns; i++) {
            if (strcmp(meta.columns[i].data_type, "int") == 0) {
                printf("%d ", *((int *)row + i));
            } else if (strcmp(meta.columns[i].data_type, "varchar") == 0) {
                printf("%s ", (char *)((void **)row)[i]);
            }
        }
        printf("\n");
    }

    free(row);
    free(meta.columns);
    fclose(file);
}
// 删除数据（这里我们将删除的操作简单化为将数据覆盖为"NULL"）
void delete_data(const char *db_name, const char *table_name, int id) {
    char table_file[100];
    snprintf(table_file, sizeof(table_file), "%s/%s.dat", db_name, table_name);
    FILE *file = fopen(table_file, "rb+");

    if (!file) {
        perror("打开表文件失败");
        return;
    }

    TableMeta meta;
    fread(&meta.num_columns, sizeof(int), 1, file);  // 读取元数据
    meta.columns = malloc(meta.num_columns * sizeof(Column));
    fread(meta.columns, sizeof(Column), meta.num_columns, file);

    void *row = malloc(meta.num_columns * sizeof(void *));
    while (fread(row, sizeof(void *), meta.num_columns, file) == meta.num_columns) {
        // 简单地假设id是第一个字段（整型）
        if (*((int *)row) == id) {
            // 将数据清空
            for (int i = 0; i < meta.num_columns; i++) {
                if (strcmp(meta.columns[i].data_type, "int") == 0) {
                    *((int *)row) = 0;
                } else if (strcmp(meta.columns[i].data_type, "varchar") == 0) {
                    memset(row, 0, strlen((char *)row) + 1);
                }
            }
            fseek(file, -meta.num_columns * sizeof(void *), SEEK_CUR);
            fwrite(row, sizeof(void *), meta.num_columns, file);
            printf("数据删除成功！\n");
            break;
        }
    }

    free(row);
    free(meta.columns);
    fclose(file);
}
void add_column(const char *db_name, const char *table_name, Column *new_column) {
    char table_file[100];
    snprintf(table_file, sizeof(table_file), "%s/%s.dat", db_name, table_name);
    FILE *file = fopen(table_file, "rb+");

    if (!file) {
        perror("打开表文件失败");
        return;
    }

    TableMeta meta;
    fread(&meta.num_columns, sizeof(int), 1, file);  // 读取元数据
    meta.columns = malloc(meta.num_columns * sizeof(Column));
    fread(meta.columns, sizeof(Column), meta.num_columns, file);

    // 增加列
    meta.num_columns++;
    meta.columns = realloc(meta.columns, meta.num_columns * sizeof(Column));
    meta.columns[meta.num_columns - 1] = *new_column;

    // 重新写入表结构
    fseek(file, 0, SEEK_SET);
    fwrite(&meta.num_columns, sizeof(int), 1, file);
    fwrite(meta.columns, sizeof(Column), meta.num_columns, file);

    fclose(file);
    free(meta.columns);

    printf("列 %s 添加成功！\n", new_column->column_name);
}
int main() {
    create_database("test_db");

    // 创建表
    TableMeta meta;
    meta.num_columns = 2;
    meta.columns = malloc(meta.num_columns * sizeof(Column));
    strcpy(meta.columns[0].column_name, "id");
    strcpy(meta.columns[0].data_type, "int");
    strcpy(meta.columns[1].column_name, "name");
    strcpy(meta.columns[1].data_type, "varchar");

    create_table("test_db", "students", &meta);

    // 插入数据
    int id = 1;
    char name[] = "Alice";
    void *data[] = {&id, name};
    insert_data("test_db", "students", data);

    // 查询数据
    query_data("test_db", "students");

    // 删除数据
    delete_data("test_db", "students", 1);

    // 查询数据
    query_data("test_db", "students");

    // 删除表
    delete_table("test_db", "students");

    // 删除数据库
    delete_database("test_db");

    free(meta.columns);
    return 0;
}
