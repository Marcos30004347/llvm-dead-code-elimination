; ModuleID = './examples/example0.ll'
source_filename = "./examples/program0.cpp"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.16.0"

; Function Attrs: noinline nounwind ssp uwtable
define i32 @_Z3foov() #0 {
bb:
  br label %bb2

bb2:                                              ; preds = %bb13, %bb
  %tmp1.0 = phi i32 [ 0, %bb ], [ %tmp8, %bb13 ]
  %tmp.0 = phi i32 [ 0, %bb ], [ %tmp15, %bb13 ]
  %tmp4 = icmp slt i32 %tmp.0, 100
  br i1 %tmp4, label %bb5, label %bb2.bb16_crit_edge

bb2.bb16_crit_edge:                               ; preds = %bb2
  br label %bb16

bb5:                                              ; preds = %bb2
  %tmp8 = add nsw i32 %tmp1.0, %tmp.0
  %tmp10 = icmp sgt i32 %tmp.0, 101
  br i1 %tmp10, label %bb11, label %bb12

bb11:                                             ; preds = %bb5
  br label %bb16

bb12:                                             ; preds = %bb5
  br label %bb13

bb13:                                             ; preds = %bb12
  %tmp15 = add nsw i32 %tmp.0, 1
  br label %bb2

bb16:                                             ; preds = %bb2.bb16_crit_edge, %bb11
  %tmp1.1 = phi i32 [ %tmp8, %bb11 ], [ %tmp1.0, %bb2.bb16_crit_edge ]
  ret i32 %tmp1.1
}

attributes #0 = { noinline nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 8.0.1 (tags/RELEASE_801/final)"}
