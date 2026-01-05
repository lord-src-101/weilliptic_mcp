import {
  WeilWallet,
  Schema,
  Contract,
  ContractFactory,
  parseSchema,
  parseExecutionResult,
  WeilWalletConnection,
  Option,
} from '@weilliptic/weil-sdk';


export const in_memory_db = ((wallet: WeilWallet | WeilWalletConnection, contractAddress: string) => ({
  create_table: async (table_name: string) => {

    const args = parseSchema(
      Schema.args({
        table_name: Schema.string,
      }),
      { table_name }
    );

    const result = await wallet.contracts.execute(
      contractAddress,
      "create_table",
      args,
    );

    return parseExecutionResult(result, Schema.i32);
  },
  drop_table: async (table_name: string) => {

    const args = parseSchema(
      Schema.args({
        table_name: Schema.string,
      }),
      { table_name }
    );

    const result = await wallet.contracts.execute(
      contractAddress,
      "drop_table",
      args,
    );

    return parseExecutionResult(result, Schema.i32);
  },
  list_tables: async () => {

    const args = {}
    const result = await wallet.contracts.execute(
      contractAddress,
      "list_tables",
      args,
    );

    return parseExecutionResult(result, Schema.array(Schema.string));
  },
  table_size: async (table_name: string) => {

    const args = parseSchema(
      Schema.args({
        table_name: Schema.string,
      }),
      { table_name }
    );

    const result = await wallet.contracts.execute(
      contractAddress,
      "table_size",
      args,
    );

    return parseExecutionResult(result, Schema.i32);
  },
  insert: async (table: string, key: string, field: string, value: string) => {

    const args = parseSchema(
      Schema.args({
        table: Schema.string,
        key: Schema.string,
        field: Schema.string,
        value: Schema.string,
      }),
      { table, key, field, value }
    );

    const result = await wallet.contracts.execute(
      contractAddress,
      "insert",
      args,
    );

    return parseExecutionResult(result, Schema.i32);
  },
  update: async (table: string, key: string, field: string, value: string) => {

    const args = parseSchema(
      Schema.args({
        table: Schema.string,
        key: Schema.string,
        field: Schema.string,
        value: Schema.string,
      }),
      { table, key, field, value }
    );

    const result = await wallet.contracts.execute(
      contractAddress,
      "update",
      args,
    );

    return parseExecutionResult(result, Schema.i32);
  },
  get_value: async (table: string, key: string, field: string) => {

    const args = parseSchema(
      Schema.args({
        table: Schema.string,
        key: Schema.string,
        field: Schema.string,
      }),
      { table, key, field }
    );

    const result = await wallet.contracts.execute(
      contractAddress,
      "get_value",
      args,
    );

    return parseExecutionResult(result, Schema.option(Schema.string));
  },
  remove_field: async (table: string, key: string, field: string) => {

    const args = parseSchema(
      Schema.args({
        table: Schema.string,
        key: Schema.string,
        field: Schema.string,
      }),
      { table, key, field }
    );

    const result = await wallet.contracts.execute(
      contractAddress,
      "remove_field",
      args,
    );

    return parseExecutionResult(result, Schema.i32);
  },
  remove_record: async (table: string, key: string) => {

    const args = parseSchema(
      Schema.args({
        table: Schema.string,
        key: Schema.string,
      }),
      { table, key }
    );

    const result = await wallet.contracts.execute(
      contractAddress,
      "remove_record",
      args,
    );

    return parseExecutionResult(result, Schema.i32);
  },
  insert_record: async (table: string, key: string, fields: [string, string][]) => {

    const args = parseSchema(
      Schema.args({
        table: Schema.string,
        key: Schema.string,
        fields: Schema.array(Schema.tuple([Schema.string, Schema.string])),
      }),
      { table, key, fields }
    );

    const result = await wallet.contracts.execute(
      contractAddress,
      "insert_record",
      args,
    );

    return parseExecutionResult(result, Schema.i32);
  },
  insert_records: async (table: string, records: [string, [string, string][]][]) => {

    const args = parseSchema(
      Schema.args({
        table: Schema.string,
        records: Schema.array(Schema.tuple([Schema.string, Schema.array(Schema.tuple([Schema.string, Schema.string]))])),
      }),
      { table, records }
    );

    const result = await wallet.contracts.execute(
      contractAddress,
      "insert_records",
      args,
    );

    return parseExecutionResult(result, Schema.i32);
  },
  get_fields: async (table: string, key: string, fields: string[]) => {

    const args = parseSchema(
      Schema.args({
        table: Schema.string,
        key: Schema.string,
        fields: Schema.array(Schema.string),
      }),
      { table, key, fields }
    );

    const result = await wallet.contracts.execute(
      contractAddress,
      "get_fields",
      args,
    );

    return parseExecutionResult(result, Schema.array(Schema.tuple([Schema.string, Schema.string])));
  },
  get_all_fields: async (table: string, key: string) => {

    const args = parseSchema(
      Schema.args({
        table: Schema.string,
        key: Schema.string,
      }),
      { table, key }
    );

    const result = await wallet.contracts.execute(
      contractAddress,
      "get_all_fields",
      args,
    );

    return parseExecutionResult(result, Schema.array(Schema.tuple([Schema.string, Schema.string])));
  },

} satisfies Contract)) satisfies ContractFactory;

