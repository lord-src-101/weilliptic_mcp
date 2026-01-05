#!/bin/bash

# 1. Configuration
WIDL_FILE="in_memory_db.widl"
RUST_BINDINGS="bindings.rs"
OUTPUT_FILE="mcp_snippets.txt"

echo "⏳ Triggering widl generation..."
# Run the command exactly as your version requires
widl generate "$WIDL_FILE" server rust

# Check if bindings.rs exists after the command
if [ ! -f "$RUST_BINDINGS" ]; then
    echo "❌ Error: $RUST_BINDINGS not found in the current directory."
    echo "Please check if 'widl' reported any errors above."
    exit 1
fi

echo "📂 Found schema in: $RUST_BINDINGS"

# 2. Extract the JSON tools schema
# It looks for the raw string block r#"[ ... ]"# inside bindings.rs
JSON_SCHEMA=$(awk '/r#"\[/,/\]"#/ {print}' "$RUST_BINDINGS" | sed 's/r#"//g' | sed 's/"#//g' | sed 's/.to_string()//g' | sed 's/;//g')

if [ -z "$JSON_SCHEMA" ]; then
    echo "❌ Error: Could not extract the JSON string from $RUST_BINDINGS."
    echo "Verify that your .widl file has @mcp tags above the interface."
    exit 1
fi

# 3. Create the snippets file
cat << EOF > "$OUTPUT_FILE"
======================================================================
MCP SNIPPETS FOR C++ (Generated from $WIDL_FILE)
======================================================================

1. PASTE THIS INSIDE YOUR CLASS IN 'contract.hpp'
--------------------------------------------------
    std::string tools() const {
        return R"JSON($JSON_SCHEMA)JSON";
    }

2. PASTE THIS INSIDE 'method_kind_data()' IN 'main.cpp'
-------------------------------------------------------
    method_kind_mapping["tools"] = "query";

3. PASTE THIS AT THE END OF YOUR EXTERN "C" BLOCK IN 'main.cpp'
--------------------------------------------------------------
extern "C" void tools() __attribute__((export_name("tools")));

void tools() {
    // 1. Recover state
    std::string stateString = weilsdk::Runtime::state();
    nlohmann::ordered_json j = nlohmann::ordered_json::parse(stateString);
    from_json(j, in_memory_db_instance);

    // 2. Get tools JSON string
    std::string tools_json_raw = in_memory_db_instance.tools();

    // 3. Wrap as a JSON string to satisfy the host's type expectation
    nlohmann::ordered_json result_wrapper = tools_json_raw;
    weilsdk::Runtime::setResult(result_wrapper.dump(), 0);
}

======================================================================
EOF

echo "✅ Success! Snippets created in: $OUTPUT_FILE"